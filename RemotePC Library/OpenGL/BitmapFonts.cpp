#include "BitmapFonts.h"

const int MaxFont = 96;

CBitmapFonts::CBitmapFonts()
{
	base = 0;
	Left = 0;
	Top  = 0;
	R = 0;
	G = 0;
	B = 0;
}

void CBitmapFonts::Build(HDC hDC, char *FontName, int *pbase, int FontSize, int l, int t, int r, int g, int b)
{
	CBitmapFontSettings Settings;
	Settings.Left = l;
	Settings.Top  = t;
	Settings.R = r;
	Settings.G = g;
	Settings.B = b;

	Build(hDC, FontName, pbase, FontSize, &Settings);
}

void CBitmapFonts::Build(HDC hDC, char *FontName, int *pbase, int FontSize, CBitmapFontSettings *pSettings)
{
	HFONT  font;	
	// Base of the font texture ID
	base = *pbase;

	SetSettings(pSettings);

	// Generate storage for 96 chars
	glGenLists(MaxFont);	

	// Create/Load the font
	font = CreateFontA(FontSize*(-1),// Height Of Font
					  0,			// Width Of Font
					  0,			// Angle Of Escapement
					  0,			// Orientation Angle
					  /*FW_THIN*/FW_BOLD,// Font Weight
					  FALSE,	// Italic
					  FALSE,	// Underline
					  FALSE,	// Strikeout
					  ANSI_CHARSET,// Character Set Identifier
					  OUT_TT_PRECIS,// Output Precision
					  CLIP_DEFAULT_PRECIS,// Clipping Precision
					  ANTIALIASED_QUALITY,// Output Quality
					  FF_DONTCARE|DEFAULT_PITCH,// Family And Pitch
					  FontName);// Font Name

	// Selects The Font We Want
	SelectObject(hDC, font);

	// Builds 96 characters starting at character 32 (space)
	wglUseFontBitmaps(hDC, 32, MaxFont, base);

	*pbase += MaxFont;
}

void CBitmapFonts::SetPosition(int L, int T)
{
	Left = L; 
	Top = T;
}

void CBitmapFonts::IncPosition(int L, int T)
{
	Left += L; 
	Top += T;
}

void CBitmapFonts::SetColor(int lR, int lG, int lB)
{
	R = lR;
	G = lG;
	B = lB;
}

void CBitmapFonts::SetSettings(CBitmapFontSettings *pSettings)
{
	// Set left and right
	Left = pSettings->Left;
	Top  = pSettings->Top;
	// Set color
	R = pSettings->R;
	G = pSettings->G;
	B = pSettings->B;

}

//Must be call manually before cleaning up opengl!
void CBitmapFonts::Free()		// Delete The Font
{
	glDeleteLists(base, MaxFont);
}

void CBitmapFonts::glPrintArg(const char *fmt, ...)
{
	if(fmt == NULL)
		return;

	va_list ap;
	char text[1024];

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPrintTxt(&text[0]);
}

void CBitmapFonts::glPrintTxt(const char *text)
{
	// Set the text color
	glColor3ub(R, G, B);
	// Position The Text On The Screen
	glRasterPos2i(Left, Top);

	glPushAttrib(GL_LIST_BIT);// Pushes The Display List Bits
	glListBase(base - 32);	// Sets The Base Character to 32
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();			// Pops The Display List Bits
}
