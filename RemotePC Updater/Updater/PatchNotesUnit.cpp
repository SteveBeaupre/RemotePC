//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PatchNotesUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPatchNotesForm *PatchNotesForm;
//---------------------------------------------------------------------------
__fastcall TPatchNotesForm::TPatchNotesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPatchNotesForm::ButtonCloseClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
