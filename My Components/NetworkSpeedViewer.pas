unit NetworkSpeedViewer;
interface
////////////////////////////////////////////////////////////////////////////////////////////
uses
  Windows, Messages, SysUtils, Classes, Controls, Graphics;
////////////////////////////////////////////////////////////////////////////////////////////
const
  NumResolutions = 4;
  NumSamples = 300;
  NumSamplesVisible = 100;
  TimeBlocksSize: Array [0..NumResolutions-1] of DWORD = (1, 5, 30, 300);
type
  TNetworkSpeedViewer = class(TCustomControl)
  private
    { Private declarations }
    FRange: Integer;
    FResolution: DWORD;
    FNumUpdates: DWORD;

    FDnSpeed: Array [0..NumResolutions-1] of Array[0..NumSamples-1] of Integer;
    FUpSpeed: Array [0..NumResolutions-1] of Array[0..NumSamples-1] of Integer;

    procedure SetResolution(AResolution: DWORD);
    procedure MoveArrayElements(Reso: Integer);
  protected
    { Protected declarations }
    procedure Paint; override;
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor  Destroy; override;

    procedure UpdateValues(DownSpeed, UpSpeed: Integer);
  published
    { Published declarations }
    property Align;
    property Canvas;
    property Color;
    property Width;
    property Height;
    property Resolution: DWORD read FResolution write SetResolution;
  end;
////////////////////////////////////////////////////////////////////////////////////////////
procedure Register;
implementation
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////CREATE THE COMPONENT//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
constructor TNetworkSpeedViewer.Create(AOwner: TComponent);
var
 i: Integer;
begin
Inherited Create(AOwner);
Width  := 100;
Height := 100;
Color  := clNavy;

FRange := 145;
FNumUpdates := 0;
FResolution := 0;
for i := 0 to NumResolutions-1 do begin
  ZeroMemory(@FDnSpeed[i][0], sizeof(Integer) * NumSamples);
  ZeroMemory(@FUpSpeed[i][0], sizeof(Integer) * NumSamples);
end;
end;
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////DESTROY THE COMPONENT/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
destructor TNetworkSpeedViewer.Destroy;
begin
Inherited Destroy;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
procedure TNetworkSpeedViewer.SetResolution(AResolution: DWORD);
begin
if(AResolution < NumResolutions) then
  FResolution := AResolution;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
procedure TNetworkSpeedViewer.MoveArrayElements(Reso: Integer);
var
 i,j: Integer;
begin
i := Reso;
for j := NumSamples-1 downto 1 do begin
  FDnSpeed[i][j] := FDnSpeed[i][j-1];
  FUpSpeed[i][j] := FUpSpeed[i][j-1];
end;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
procedure TNetworkSpeedViewer.UpdateValues(DownSpeed, UpSpeed: Integer);
var
 i,j: Integer;
 d,u: Integer;
begin
// Copy values to next slot in the array
i := 0;
MoveArrayElements(i);
// Update current values
FDnSpeed[0][0] := DownSpeed;
FUpSpeed[0][0] := UpSpeed;

FRange := 1;
for j := 0 to NumSamplesVisible do begin
  if(FDnSpeed[0][j] > FRange) then FRange := FDnSpeed[0][j];
  if(FUpSpeed[0][j] > FRange) then FRange := FUpSpeed[0][j];
end;

// Inc. the updates counter
Inc(FNumUpdates);

// Update remaining values
for i := 1 to NumResolutions-1 do begin
  if(FNumUpdates mod TimeBlocksSize[i] = 0) then begin

    // Copy values to next slot in the array
    MoveArrayElements(i);

    // Average values
    d := 0;
    u := 0;
    for j := 0 to TimeBlocksSize[i]-1 do begin
      Inc(d, FDnSpeed[i-1][j]);
      Inc(u, FUpSpeed[i-1][j]);
    end;

    d := Trunc(d / TimeBlocksSize[i]);
    u := Trunc(u / TimeBlocksSize[i]);
    
    // Update values
    FDnSpeed[i][0] := d;
    FUpSpeed[i][0] := u;
  end;
end;
Paint;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
procedure TNetworkSpeedViewer.Paint;
const
 BorderSize = 30;
var
 s: String;
 r: TRect;
 i,j,k,v: Integer;
 x,y,w,h: Integer;
begin
// Erase previous stuffs
Canvas.Brush.Color := Color;
Canvas.FillRect(Self.GetClientRect);

// Initialize some variables
v := 0;
w := Width  - (BorderSize*2);
h := Height - (BorderSize*2);

// Draw the main border
Canvas.Pen.Color := $00C0C0C0;
Canvas.MoveTo(BorderSize,   BorderSize);
Canvas.LineTo(BorderSize+w, BorderSize);
Canvas.LineTo(BorderSize+w, BorderSize+h);
Canvas.LineTo(BorderSize,   BorderSize+h);
Canvas.LineTo(BorderSize,   BorderSize);

Canvas.Pen.Color := $00B00000;
Canvas.Pen.Style := psDash;
// Draw horizontal dotted lines
for i := 1 to 3 do begin
  y := Trunc(BorderSize + ((h / 4) * i));
  Canvas.MoveTo(BorderSize+1,   y);
  Canvas.LineTo(BorderSize+w-1, y);
end;
// Draw vertical dotted lines
for i := 1 to 9 do begin
  x := Trunc(BorderSize + ((w / 10) * i));
  Canvas.MoveTo(x, BorderSize+1);
  Canvas.LineTo(x, BorderSize+h-1);
end;
Canvas.Pen.Style := psSolid;

// Draw some text
r.Left   := 3;
r.Right  := BorderSize-3;
r.Top    := BorderSize;
r.Bottom := r.Top + 10;
Canvas.Font.Color := clWhite;
s := IntToStr(FRange div 1024);
DrawText(Canvas.Handle, PCHAR(s), Length(s), r, (DT_RIGHT or DT_VCENTER or DT_SINGLELINE));

r.Left   := 3;
r.Right  := BorderSize-3;
r.Bottom := Height - BorderSize;
r.Top    := r.Bottom - 10;
Canvas.Font.Color := clWhite;
DrawText(Canvas.Handle, PCHAR(IntToStr(0)), 1, r, (DT_RIGHT or DT_VCENTER or DT_SINGLELINE));

i := FResolution;
// Draw the data
for k := 0 to 1 do begin
  for j := 0 to NumSamplesVisible do begin

    // Choose drawing color and store value into v
    case(k = 0) of
      True:  begin Canvas.Pen.Color := clRed;  v := FDnSpeed[i][j]; end;
      False: begin Canvas.Pen.Color := clLime; v := FUpSpeed[i][j]; end;
    end;

    // Calc. the x,y coordinates
    x := (w - Trunc(j * (w / NumSamplesVisible))) + BorderSize;
    y := (Height-1) - (BorderSize + (Trunc((v / FRange) * h)));

    // Draw the line
    case(j = 0) of
      True:  Canvas.MoveTo(x,y);
      False: Canvas.LineTo(x,y);
    end;
  end;
end;

Canvas.Pen.Color   := clWhite;
Canvas.Brush.Color := clWhite;
end;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
procedure Register;
begin
RegisterComponents('MyStuffs', [TNetworkSpeedViewer]);
end;
////////////////////////////////////////////////////////////////////////////////////////////
end.
