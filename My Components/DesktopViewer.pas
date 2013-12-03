unit DesktopViewer;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls;

type
  TOnMouseRollEvent = procedure(Sender: TObject; WheelDelta: SmallInt) of object;

  {TKeyParamsEvent   = procedure(Sender: TObject; wParam, lParam: DWORD) of object;
  TOnKeyDownParamsEvent    = TKeyParamsEvent;
  TOnKeyUpParamsEvent      = TKeyParamsEvent;
  TOnSysKeyDownParamsEvent = TKeyParamsEvent;
  TOnSysKeyUpParamsEvent   = TKeyParamsEvent;}
  
  TDesktopViewer = class(TCustomControl)
  private
    { Private declarations }
    FOnMouseRoll:             TOnMouseRollEvent;

    {FOnKeyDownParamsEvent:    TOnKeyDownParamsEvent;
    FOnKeyUpParamsEvent:      TOnKeyUpParamsEvent;
    FOnSysKeyDownParamsEvent: TOnSysKeyDownParamsEvent;
    FOnSysKeyUpParamsEvent:   TOnSysKeyUpParamsEvent;}
  protected
    { Protected declarations }
    procedure WmMouseWheel(var Msg: TWMMouseWheel);  message WM_MOUSEWHEEL;
    {procedure WmKeyDownParams(var Msg: TMessage);    message WM_KEYDOWN;
    procedure WmKeyUpParams(var Msg: TMessage);      message WM_KEYUP;
    procedure WmSysKeyDownParams(var Msg: TMessage); message WM_SYSKEYDOWN;
    procedure WmSysKeyUpParams(var Msg: TMessage);   message WM_SYSKEYUP;}
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
  published
    { Published declarations }
    property Align;
    property Color;
    property Width;
    property Height;
    property OnKeyDown;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseUp;
    property OnMouseMove;

    property OnMouseRoll: TOnMouseRollEvent read FOnMouseRoll write FOnMouseRoll;

    {property OnKeyDownParams:    TOnKeyDownParamsEvent    read FOnKeyDownParamsEvent    write FOnKeyDownParamsEvent;
    property OnKeyUpParams:      TOnKeyUpParamsEvent      read FOnKeyUpParamsEvent      write FOnKeyUpParamsEvent;
    property OnSysKeyDownParams: TOnSysKeyDownParamsEvent read FOnSysKeyDownParamsEvent write FOnSysKeyDownParamsEvent;
    property OnSysKeyUpParams:   TOnSysKeyUpParamsEvent   read FOnSysKeyUpParamsEvent   write FOnSysKeyUpParamsEvent;}
  end;

procedure Register;

implementation

constructor TDesktopViewer.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
Color := 0;
end;

destructor TDesktopViewer.Destroy;
begin
Inherited Destroy;
end;

procedure TDesktopViewer.WmMouseWheel(var Msg: TWMMOUSEWHEEL);
begin
if(Assigned(OnMouseRoll)) then
  OnMouseRoll(Self, Msg.WheelDelta);
end;

{procedure TDesktopViewer.WmKeyDownParams(var Msg: TMessage);
begin
if(Assigned(OnKeyDownParams)) then
  OnKeyDownParams(Self, Msg.WParam, Msg.LParam);
end;

procedure TDesktopViewer.WmKeyUpParams(var Msg: TMessage);
begin
if(Assigned(OnKeyUpParams)) then
  OnKeyUpParams(Self, Msg.WParam, Msg.LParam);
end;

procedure TDesktopViewer.WmSysKeyDownParams(var Msg: TMessage);
begin
if(Assigned(OnSysKeyDownParams)) then
  OnSysKeyDownParams(Self, Msg.WParam, Msg.LParam);
end;

procedure TDesktopViewer.WmSysKeyUpParams(var Msg: TMessage);
begin
if(Assigned(OnSysKeyUpParams)) then
  OnSysKeyUpParams(Self, Msg.WParam, Msg.LParam);
end;}

procedure Register;
begin
  RegisterComponents('MyStuffs', [TDesktopViewer]);
end;

end.
