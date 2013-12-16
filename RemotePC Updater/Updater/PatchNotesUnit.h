//---------------------------------------------------------------------------

#ifndef PatchNotesUnitH
#define PatchNotesUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TPatchNotesForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox;
	TButton *ButtonClose;
	void __fastcall ButtonCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPatchNotesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPatchNotesForm *PatchNotesForm;
//---------------------------------------------------------------------------
#endif
