unit DesktopViewer;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls;

type
  TOnMouseRollEvent = procedure(Sender: TObject; Dir: SmallInt) of object;
  
  TDesktopViewer = class(TCustomControl)
  private
    { Private declarations }
    FOnMouseRoll: TOnMouseRollEvent;
  protected
    { Protected declarations }
    procedure WmMouseWheel(var Msg : TWMMouseWheel); message WM_MOUSEWHEEL;
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;
  published
    { Published declarations }
    property Align;
    property Width;
    property Height;
    property OnKeyDown;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseUp;
    property OnMouseMove;
    property OnMouseRoll: TOnMouseRollEvent read FOnMouseRoll write FOnMouseRoll;
  end;

procedure Register;

implementation

constructor TDesktopViewer.Create(AOwner: TComponent);
begin
inherited Create(AOwner);
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

procedure Register;
begin
  RegisterComponents('MyStuffs', [TDesktopViewer]);
end;

end.
