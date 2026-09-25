#ifndef __ARCHIPELAGODIALOG_H__
#define __ARCHIPELAGODIALOG_H__

#include "MoneyDialog.h"
#include "SexyAppFramework/EditListener.h"

namespace Sexy
{
	class WinFishApp;
	class EditWidget;
	class UserProfile;

	// Edits the Archipelago server, slot and password of one profile.
	class ArchipelagoDialog : public MoneyDialog, public ButtonListener, public EditListener
	{
	public:
		enum
		{
			EDIT_SERVER,
			EDIT_SLOT,
			EDIT_PASSWORD,
			NUM_EDITS
		};

		SexyString				mUserName;
		EditWidget*				mEditWidgets[NUM_EDITS];

	public:
		ArchipelagoDialog(WinFishApp* theApp, const SexyString& theUserName, UserProfile* theProfile);
		virtual ~ArchipelagoDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int				GetPreferredHeight(int theWidth);

		virtual void			ButtonPress(int theId);

		virtual void			EditWidgetText(int theId, const SexyString& theString);

		// Copies the (trimmed) fields into theProfile.
		void					ApplyTo(UserProfile* theProfile);
	};
}

#endif
