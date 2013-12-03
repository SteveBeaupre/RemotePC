unit KeyEdit;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, ExtCtrls;

type
    TKeyEdit = class(TCustomEdit)
    private
      FVirtualKeyCode: Integer;
      FExtendedKey: Boolean;

   protected
      procedure WMKeyDown(var Msg: TWMKey); message WM_KEYDOWN;
      procedure SetVirtualKeyCode( Value: Integer );
      procedure SetExtendedKey( Value: Boolean );

	public
  		constructor Create(AOwner: TComponent); override;

	published
      property VirtualKeyCode: Integer read FVirtualKeyCode
         write SetVirtualKeyCode;
      property ExtendedKey: Boolean read FExtendedKey write SetExtendedKey;

      // atention: não insert the CharCase property because it causes a problem
      // when in UpperCase or LowerCase, that seems related with a bug in the
      // VCL code and not in this.
      property BorderStyle;
    	property Color;
      property Ctl3D;
      property Cursor;
      property DragCursor;
      property DragMode;
		property Enabled;
		property Font;
      property Hint;
		property ParentColor;
      property ParentCtl3D;
		property ParentFont;
		property ParentShowHint;
      property PopupMenu;
		property ShowHint;
      property TabOrder;
      property TabStop;
      property Tag;
      property Text;
		property Visible;

      property OnChange;
      property OnClick;
      property OnDblClick;
      property OnDragDrop;
      property OnDragOver;
      property OnEndDrag;
      property OnEnter;
      property OnExit;
      property OnKeyDown;
      property OnKeyPress;
      property OnKeyUp;
      property OnMouseDown;
      property OnMouseMove;
      property OnMouseUp;
      property OnStartDrag;
    end;


Procedure Register;


implementation


procedure Register;
begin
	RegisterComponents('MyStuffs', [TKeyEdit]);
end;


constructor TKeyEdit.Create(AOwner: TComponent);
begin
   inherited Create(AOwner);

   FExtendedKey := False;
   FVirtualKeyCode := -1;
   ReadOnly := True;
   Text := '(none)';
end;


procedure TKeyEdit.WMKeyDown(var Msg: TWMKey);
begin
   inherited DoKeyDown( Msg );

   if ((Msg.KeyData and $1000000) <> 0) then
      FExtendedKey := True
   else
      FExtendedKey := False;

   VirtualKeyCode := Msg.CharCode;
end;


procedure TKeyEdit.SetVirtualKeyCode( Value: Integer );
var
   st: array [0..50] of char;
   ScanCode: Integer;
   KeyData: Longint;
begin
   ScanCode := MapVirtualKey( Value, 0 );
   KeyData := Longint(ScanCode) shl 16;
   if (FExtendedKey = True) then
      KeyData := KeyData or $1000000;

 	if (ScanCode = 0) or (GetKeyNameText( KeyData, @st, 50 ) = 0) then
   begin
      SetTextBuf('(none)');
      FVirtualKeyCode := -1;
   end
   else
   begin
      SetTextBuf( st );
	  	FVirtualKeyCode := Value;
   end;
end;


procedure TKeyEdit.SetExtendedKey( Value: Boolean );
begin
   FExtendedKey := Value;
   VirtualKeyCode := FVirtualKeyCode;
end;

end.
