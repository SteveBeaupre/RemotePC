unit IPEdit;

{ This is a simple IP Address Editor component. Many traditional TEdit Methots/Properties
  are supported. One major difference is the 'Value' property which gives the CARDINAL value
  of a TCP/IP address (192.168.1.1 becomes $C0A80101 or 3232235777). This makes to/from range
  enty a snap.
  Modelled by bhoc@surfeu.ch.
  This is GPL Code.
}  

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TIPEdit = class;
    TIPByteEdit = class(TEdit)
    private
      { Private declarations }
      MyAlignment: TAlignment;
      procedure SetAlignment(Value: TAlignment);
    protected
      { Protected declarations }
      procedure CreateParams(var Params: TCreateParams); override;
    public
    { Public declarations }
    published
      { Published declarations }
      ParentPanel: TIPEdit;
      constructor Create(AOwner: TComponent); override;
      Property Alignment: TAlignment read MyAlignment write SetAlignment;
    end;
  TIPEdit = class(TCustomPanel)
  private
    { Private declarations }
    MyEnabled: Boolean;
    InternalMove: Boolean;
    MyChange: TNotifyEvent;
    MyEnter: TNotifyEvent;
    procedure PanelEnter(Sender: TObject);
    procedure MyResize(Sender: TObject);
    procedure PanelClick(Sender: TObject);
    procedure PanelDblClick(Sender: TObject);
    procedure FilterKey(Sender: TObject; var Key: Char);
    procedure ExitByteEdit(Sender: TObject);
    procedure EditClick(Sender: TObject);
    procedure EditChange(Sender: TObject);
    function GetText: String;
    procedure SetText(Value: String);
    function GetValue: Cardinal;
    procedure SetValue(Value: Cardinal);
    procedure Set_Enabled(TrueFalse: Boolean);
  protected
    { Protected declarations }
    IPByteEdit: Array[0..3] of TIPByteEdit;
    Dots: Array[0..3] of TLabel;
  public
  { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Loaded; override;
  published
    { Published declarations }
    property Ctl3D stored True;
    property Font;
    property Text: String read GetText write SetText stored True;
    property Value: Cardinal read GetValue write SetValue stored True;
    property Visible;
    property Anchors;
    property BorderStyle;
    property Color;
    property ShowHint;
    property Enabled: boolean read MyEnabled write Set_Enabled;
    property OnChange: TNotifyEvent read MyChange write MyChange;
    property OnClick;
    property OnDblClick;
    property OnEnter: TNotifyEvent read MyEnter write MyEnter;
    property OnExit;
    property TabOrder;
    property TabStop;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('MyStuffs', [TIPEdit]);
end;

procedure TIPEdit.Loaded;
begin
  inherited loaded;
  MyResize(Self);
end;

destructor TIpEdit.Destroy;
var n: integer;
begin
  for n := 0 to 3 do
  begin
    IPByteEdit[n].Free;
    Dots[n].Free;
  end;
  Inherited Destroy;
end;

constructor TIpEdit.Create(AOwner: TComponent);
var n: integer;
begin
  inherited Create(AOwner);
  inherited Enabled := True;
  inherited OnEnter := PanelEnter;
  OnEnter := nil;
  OnChange := nil;
  MyEnabled := True;
  Caption := '';
  Height := 21;
  InternalMove := False;
  BevelInner := bvNone;
  BevelOuter := bvNone;
  BorderStyle := bsSingle;
  OnResize := MyResize;
  Color := clWindow;
  for n := 0 to 3 do
  begin
    IPByteEdit[n] := TIPByteEdit.Create(Self);
    IPByteEdit[n].Parent := Self;
    IPByteEdit[n].Tag := n;
    IPByteEdit[n].MaxLength := 3;
    IPByteEdit[n].OnKeyPress := FilterKey;
    IPByteEdit[n].OnExit := ExitByteEdit;
    IPByteEdit[n].OnClick := EditClick;
    IPByteEdit[n].OnDblClick := PanelDblClick;
    IPByteEdit[n].ParentColor := True;
    IPByteEdit[n].ParentFont := True;
    IPByteEdit[n].ParentShowHint := True;
    IPByteEdit[n].OnChange := EditChange;
    Dots[n] := TLabel.Create(Self);
    Dots[n].Parent := Self;
    Dots[n].ParentColor := True;
    Dots[n].ParentFont := True;
    Dots[n].ParentShowHint := True;
    Dots[n].OnClick := PanelClick;
    Dots[n].OnDblClick := PanelDblClick;
    Dots[n].Caption := '.';
    Dots[n].AutoSize := True;
    Dots[n].Alignment := taCenter;
  end;
  Dots[3].Caption := '';
  Dots[3].Visible := False;
end;

procedure TIPEdit.PanelEnter(Sender: TObject);
begin
  IPByteEdit[0].SetFocus;
  if assigned(OnEnter) then OnEnter(Self);
end;

procedure TIPEdit.EditClick(Sender: TObject);
begin
  if InternalMove then
    InternalMove := False
  else
    if assigned(inherited OnClick) then OnClick(Self);
end;

procedure TIPEdit.EditChange(Sender: TObject);
begin
  if assigned(OnChange) then OnChange(Self);
end;

procedure TIPEdit.PanelClick(Sender: TObject);
begin
  if assigned(Inherited OnClick) then OnClick(Self);
end;

procedure TIPEdit.PanelDblClick(Sender: TObject);
begin
  if assigned(Inherited OnDblClick) then OnDblClick(Self);
end;

procedure TIPEdit.FilterKey(Sender: TObject; var Key: Char);
var n: integer;
begin
  if Sender is TIPByteEdit then
  begin
    case Key of
    '.': begin
        n := TIPBYteEdit(Sender).Tag;
        key := #0;
        case n of
          0..2: begin
                  InternalMove := True;
                  IPByteEdit[n+1].SetFocus;
                end;
        end;
      end;
    '0'..'9', #8: // do nothing special
    else
      key := #0;
    end;
  end;
end;

procedure TIPEdit.Set_Enabled(TrueFalse: Boolean);
var n: integer;
begin
  inherited Enabled := TrueFalse;
  MyEnabled := TrueFalse;
  for n := 0 to 3 do
  begin
    IPByteEdit[n].Enabled := TrueFalse;
    Dots[n].Enabled := TrueFalse;
  end;
end;

procedure TIPEdit.ExitByteEdit(Sender: TObject);
begin
  if Sender is TIPByteEdit then
  begin
    TIPByteEdit(Sender).Text := IntToStr(StrToInt(TIPByteEdit(Sender).Text) mod 256);
  end;
end;

function TIPEdit.GetText: String;
begin
  Result := Concat(IPByteEdit[0].Text, '.',
                   IPByteEdit[1].Text, '.',
                   IPByteEdit[2].Text, '.',
                   IPByteEdit[3].Text);
end;

Procedure TIPEdit.SetText(Value: String);
var p: integer;
    n: integer;

    function mod256(sByte: String): string;
    var i, x: integer;
    begin
      Val(sByte, i, x);
      {$WARN IMPLICIT_STRING_CAST OFF}
      Str((i Mod 256), Result);
      {$WARN IMPLICIT_STRING_CAST ON}
    end;

begin
  Value := Trim(Value) + '.';
  p := pos('.', Value);
  n := 0;
  while p > 0 do
  begin
    IPByteEdit[n].Text := mod256(Copy(Value, 1, p-1));
    inc(n);
    if n > 3 then exit;
    Value := Copy(Value, p+1, length(Value) - p);
    p := pos('.', Value);
  end;
end;

function TIPEdit.GetValue: Cardinal;
begin
  Result := (StrToInt(IPByteEdit[0].Text) shl 24) or
            (StrToInt(IPByteEdit[1].Text) shl 16) or
            (StrToInt(IPByteEdit[2].Text) shl 8) or
            StrToInt(IPByteEdit[3].Text);
end;

procedure TIPEdit.SetValue(Value: Cardinal);
var n: integer;
begin
  for n := 3 downto 0 do
  begin
    IPByteEdit[n].Text := IntToStr(Value and $FF);
    Value := Value shr 8;
  end;
end;

procedure TIPEdit.MyResize(Sender: TObject);
var n: integer;
    dw: integer;
    ew: integer;
    TotWidth: integer;
    BorderWidth: Integer;
    ewMin: integer;
    hMin: integer;
begin
  BorderWidth := Width - ClientWidth;
  ewMin := Dots[0].Canvas.TextWidth('000');
  hMin := Dots[0].Canvas.TextHeight('0');
  dw := Dots[0].Width;
  ew := (Width - BorderWidth - dw - dw - dw) div 4;
  if ew < ewMin then ew := ewMin;
  TotWidth := ew + ew + ew + ew + dw + dw + dw + BorderWidth;
  if Height < (hMin + BorderWidth) then
  begin
    Height := hMin + BorderWidth;
  end else
    if Width < TotWidth then
    begin
      Width := TotWidth;
    end else
      for n := 0 to 3 do
      begin
        IPByteEdit[n].Width := ew;
        IPByteEdit[n].Height := hMin;
        IPByteEdit[n].Left := n * (ew + dw);
        IPByteEdit[n].Top := (ClientHeight - hMin) div 2;
        Dots[n].Height := IPByteEdit[n].Height;
        Dots[n].Top := IPByteEdit[n].Top;
        Dots[n].Left := ew + n * (ew + dw);
      end;
end;

constructor TIPByteEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  BorderStyle := bsNone;
  Text := '0';
  Alignment := taCenter;
end;

procedure TIPByteEdit.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  case Alignment of
    taLeftJustify : Params.Style := Params.Style or ES_LEFT and Not ES_MULTILINE;
    taRightJustify : Params.Style := Params.Style or ES_RIGHT and Not ES_MULTILINE;
    taCenter : Params.Style := Params.Style or ES_CENTER and Not ES_MULTILINE;
  end;
end;

procedure TIPByteEdit.SetAlignment(Value: TAlignment);
begin
  if MyAlignment <> Value then
  begin
    MyAlignment := Value;
    RecreateWnd;
  end;
end;

end.
