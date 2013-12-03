//A unit providing the tray icon component.

//Created by Tomas Koutny.
//Version 1.5

//------------------------------------------------------------------------------
//WARRANTIES AND DISCLAIMER:
//
//EXCEPT AS EXPRESSLY PROVIDED OTHERWISE IN A WRITTEN AGREEMENT BETWEEN
//Tomas Koutny ("LICENSOR") AND YOU, THE LICENSED WORKS ARE NOW PROVIDED "AS IS"
//WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS
//FOR A PARTICULAR PURPOSE, OR THE WARRANTY OF NON-INFRINGEMENT.

//IN NO EVENT SHALL LICENSOR OR ITS SUPPLIERS BE LIABLE TO YOU OR ANY THIRD
//PARTY FOR ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
//KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING, WITHOUT LIMITATION, THOSE
//RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT LICENSOR HAD BEEN
//ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, AND ON ANY THEORY OF LIABILITY,
//ARISING OUT OF OR IN CONNECTION WITH THE USE OF THE LICENSED WORKS. SOME
//JURISDICTIONS PROHIBIT THE EXCLUSION OR LIMITATION OF LIABILITY FOR
//CONSEQUENTIAL OR INCIDENTAL DAMAGES, SO THE ABOVE LIMITATIONS MAY NOT APPLY TO
//YOU. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL
//PURPOSE OF ANY LIMITED REMEDY.
//------------------------------------------------------------------------------

//See www.rawos.com for more software and resources.

unit TrayIcon;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, ShellAPI,
  menus;

type
  TTrayIcon = class(TComponent)
  private
    procedure WndProc(var msg : TMessage); //Message handler
  protected
    FIcon:TIcon;                  //An picture to show in the tray bar
    FNotifyData: TNotifyIconData; //Data representing tray icon to the Win32
    FIconInTray:Boolean;          //True if the icon is in the tray bar
    FPopupMenu:TPopupMenu;        //Pop-up menu
    FWindowHandle:THandle;        //Component's Win32 handle

    FOnDblClick,
    FOnClick:TNotifyEvent;        //Events given to the user

    procedure Add;                //Adds the icon into the tray bar
    procedure Remove;             //Removes the icon from the tray bar
    procedure Modify;             //Modifies icon's properties
    procedure SetVisibility(ShowIt:boolean);
                                  //Calls either Add or Remove
    procedure SetIcon(NewIcon:TIcon); //Sets a new picture
    function GetHint:string;      //Returns used hint text
    procedure SetHint(NewHint:string); //Sets a new hint text
  public
    constructor Create(AOwner:TComponent); override;
    destructor Destroy; override;

    procedure UpdateIcon;

    procedure Show(Always: boolean);               //Visible:=true
    procedure Hide;               //Visible:=false

    procedure ForceAdd;           //Tries to add icon at all costs
                                  //needed for RM_TASKBAR_CREATED

    procedure ShowPopup;          //Invokes a pop-up menu if possible
  published
    property Visible:Boolean read FIconInTray write SetVisibility;
    property PopupMenu:TPopupMenu read FPopupMenu write FPopupMenu;
    property Icon:TIcon read FIcon write SetIcon;
    property Hint:String read GetHint write SetHint;

    property OnDblClick:TNotifyEvent read FOnDblClick write FOnDblClick;
    property OnClick:TNotifyEvent read FOnClick write FOnClick;
  end;

procedure Register;

implementation

//{$R *.DCR}

const
  WM_ToolTrayIcon = WM_User+1;       //Callback message identification
  WM_SetToolTip   = WM_User+2;       //Message to set up a hint text   

procedure TTrayIcon.UpdateIcon;
begin
Modify;
end;

procedure TTrayIcon.SetIcon(NewIcon:TIcon);
begin
  FIcon.Assign(NewIcon);

  Modify; //Icon handle assignment will be done in the Modify method
end;

function TTrayIcon.GetHint:string;
begin
  result:=string(FNotifyData.szTip);
end;

procedure TTrayIcon.SetHint(newHint:string);
begin
  //Keep the hint length at maximum of 64 characters 
  if Length(NewHint) > 64 then NewHint:=copy(NewHint, 1, 64);
  StrPCopy(FNotifyData.szTip, NewHint);

  Modify;
end;


procedure TTrayIcon.Add;
begin
  FIconInTray:=Shell_NotifyIcon(NIM_ADD, @FNotifyData);

  PostMessage(FWindowHandle, WM_SetToolTip, 0, 0); //A Windows speciality;
                //if tip is an empty string, the tray icon must not get visible
end;

procedure TTrayIcon.Remove;
begin
  FIconInTray:=not Shell_NotifyIcon(NIM_DELETE, @FNotifyData);
end;

procedure TTrayIcon.Modify;
begin
  FNotifyData.hIcon:=FIcon.Handle; //The Win32 system requires always actual
                                   //and FIcon.Handle could change

  if FIconInTray then Shell_NotifyIcon(NIM_MODIFY, @FNotifyData);
end;

procedure TTrayIcon.SetVisibility(ShowIt:boolean);
begin
  //No tray bar utilization in the design state
  if csDesigning in ComponentState then
     begin
       FIconInTray:=ShowIt;
       exit;
     end;

  //Determine what to do in the depence on the current state and ShowIt
  if not(ShowIt) and (FIconInTray) then Remove
     else if ShowIt and not(FIconInTray) then Add;
end;

{$WARN SYMBOL_DEPRECATED OFF}
constructor TTrayIcon.Create(AOwner:TComponent);
begin
  inherited;


  //Allocate and setup resources
  FWindowHandle := AllocateHWnd(WndProc);
  
  FIcon:=TIcon.Create;
  FIconInTray:=false;

  with FNotifyData do
     begin
       cbSize:=sizeof(TNotifyIconData);
       Wnd:=FWindowHandle;
       uFlags := NIF_MESSAGE or NIF_ICON or NIF_TIP;
       hIcon := FIcon.Handle;
       uCallbackMessage:=WM_ToolTrayIcon;
       uID:=0;
     end;
end;


destructor TTrayIcon.Destroy;
begin
  Remove;                                  //Remove the icon from the tray bar

  FIcon.Free;                              //and free allocated resources
  DeAllocateHWnd(FWindowHandle);

  inherited;
end;
{$WARN SYMBOL_DEPRECATED ON}

procedure TTrayIcon.ShowPopup;
var MouseAt:TPoint;
begin
  if Assigned(FPopupMenu) then
     begin
       GetCursorPos(MouseAt);                    //Where to pop-up the menu
       SetForegroundWindow(FWindowHandle);  //Show application
       //Application.ProcessMessages;              //Let Win32 act
       FPopupmenu.Popup(MouseAt.X, MouseAt.Y);   //Pop-up the menu
       //PostMessage(FWindowHandle, WM_NULL, 0, 0);
     end;
end;

procedure TTrayIcon.WndProc(var msg : TMessage);
begin
   with msg do
     if msg = WM_SetToolTip then Modify         //Modify icon properties
        else if msg = WM_ToolTrayIcon then      //React on event
          begin
            case lParam of
              WM_LBUTTONDBLCLK:if assigned(FOnDblClick) then FOnDblClick(self);
              WM_LBUTTONUP    :if assigned(FOnClick) then FOnClick(self);
              WM_RBUTTONUP    :ShowPopup;
            end;
          end else                              //Not interested in this event
            Result := DefWindowProc(FWindowHandle, Msg, wParam, lParam);
end;

procedure TTrayIcon.ForceAdd;
begin
  Add;
end;

procedure TTrayIcon.Show(Always: boolean);
begin
  if not Always then Visible:=true 
    else Add;
end;

procedure TTrayIcon.Hide;
begin
  Visible:=false;
end;

procedure Register;
begin
  RegisterComponents('MyStuffs', [TTrayIcon]);
end;

end.
