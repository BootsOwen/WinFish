#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/EditWidget.h>

#include "ArchipelagoDialog.h"
#include "WinFishApp.h"
#include "WinFishCommon.h"
#include "ProfileMgr.h"
#include "Res.h"

using namespace Sexy;

static const char* const gArchipelagoLabels[ArchipelagoDialog::NUM_EDITS] =
{
	"SERVER (HOST:PORT)",
	"SLOT NAME",
	"PASSWORD",
};

Sexy::ArchipelagoDialog::ArchipelagoDialog(WinFishApp* theApp, const SexyString& theUserName, UserProfile* theProfile) :
	MoneyDialog(theApp, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, DIALOG_ARCHIPELAGO, true, "ARCHIPELAGO",
		"Multiworld settings for " + theUserName + ".\nLeave the slot empty to play normally.", "", BUTTONS_OK_CANCEL)
{
	SetButtonFont(FONT_JUNGLEFEVER12OUTLINE);
	SetHeaderFont(FONT_JUNGLEFEVER15OUTLINE);
	SetLinesFont(FONT_JUNGLEFEVER10OUTLINE);
	mContentInsets = Insets(36, 15, 36, 36);
	SetColor(COLOR_HEADER, Color(0xff, 200, 0));
	SetColor(COLOR_LINES, Color::White);
	SetColor(COLOR_BUTTON_TEXT, Color::White);
	mUserName = theUserName;

	const std::string* aValues[NUM_EDITS] = { &theProfile->mAPServer, &theProfile->mAPSlot, &theProfile->mAPPassword };
	for (int i = 0; i < NUM_EDITS; i++)
	{
		mEditWidgets[i] = MakeEditWidget(i, this);
		mEditWidgets[i]->mMaxChars = 64;
		mEditWidgets[i]->SetText(*aValues[i], true);
		mEditWidgets[i]->mCursorPos = mEditWidgets[i]->mString.size();
	}
	mEditWidgets[EDIT_SLOT]->mMaxChars = 16; // Archipelago's slot name limit
	mEditWidgets[EDIT_PASSWORD]->mPasswordChar = '*';

	for (int i = 0; i < NUM_EDITS; i++)
	{
		mEditWidgets[i]->mTabNext = mEditWidgets[(i + 1) % NUM_EDITS];
		mEditWidgets[i]->mTabPrev = mEditWidgets[(i + NUM_EDITS - 1) % NUM_EDITS];
	}
}

Sexy::ArchipelagoDialog::~ArchipelagoDialog()
{
	for (int i = 0; i < NUM_EDITS; i++)
		delete mEditWidgets[i];
}

void Sexy::ArchipelagoDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::AddedToManager(theWidgetManager);
	for (int i = 0; i < NUM_EDITS; i++)
		theWidgetManager->AddWidget(mEditWidgets[i]);
	theWidgetManager->SetFocus(mEditWidgets[EDIT_SERVER]);
}

void Sexy::ArchipelagoDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	MoneyDialog::RemovedFromManager(theWidgetManager);
	for (int i = 0; i < NUM_EDITS; i++)
		theWidgetManager->RemoveWidget(mEditWidgets[i]);
}

void Sexy::ArchipelagoDialog::Draw(Graphics* g)
{
	MoneyDialog::Draw(g);
	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color(0xff, 200, 0));
	for (int i = 0; i < NUM_EDITS; i++)
	{
		g->DrawString(gArchipelagoLabels[i], mEditWidgets[i]->mX - mX, mEditWidgets[i]->mY - mY - 6);
		DrawEditWidgetBox(g, mEditWidgets[i]);
	}
}

void Sexy::ArchipelagoDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	MoneyDialog::Resize(theX, theY, theWidth, theHeight);
	int anY = theY + 130;
	for (int i = 0; i < NUM_EDITS; i++)
	{
		mEditWidgets[i]->Resize(theX + 48, anY, theWidth - 96, 24);
		anY += 24 + 34;
	}
}

int Sexy::ArchipelagoDialog::GetPreferredHeight(int theWidth)
{
	return MoneyDialog::GetPreferredHeight(theWidth) + NUM_EDITS * (24 + 34);
}

void Sexy::ArchipelagoDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::ArchipelagoDialog::EditWidgetText(int theId, const SexyString& theString)
{
	// Enter moves to the next field; on the last field it presses OK.
	if (theId + 1 < NUM_EDITS)
		mWidgetManager->SetFocus(mEditWidgets[theId + 1]);
	else
		MoneyDialog::ButtonDepress(ID_YES);
}

void Sexy::ArchipelagoDialog::ApplyTo(UserProfile* theProfile)
{
	theProfile->mAPServer = Trim(mEditWidgets[EDIT_SERVER]->mString);
	theProfile->mAPSlot = Trim(mEditWidgets[EDIT_SLOT]->mString);
	theProfile->mAPPassword = mEditWidgets[EDIT_PASSWORD]->mString;
}
