// apclient.hpp must come before anything that includes windows.h (see APBridge.h).
#include <apclient.hpp>
#include <apuuid.hpp>

#include "APBridge.h"

using namespace Sexy;

// Must match `game` in Archipelago/worlds/insaniquarium.
static const char* const AP_GAME_NAME = "Insaniquarium Deluxe";

// Remote items from other worlds, our own world, and starting inventory.
static const int AP_ITEMS_HANDLING = 0b111;

APBridge::APBridge(const std::string& theDataFolder, const std::string& theCertFile)
{
	mClient = NULL;
	mState = AP_DISCONNECTED;
	mDataFolder = theDataFolder;
	mCertFile = theCertFile;
}

APBridge::~APBridge()
{
	Disconnect();
}

void APBridge::Connect(const std::string& theServer, const std::string& theSlot, const std::string& thePassword)
{
	Disconnect();

	mSlot = theSlot;
	mPassword = thePassword;
	mLastError.clear();

	std::string aUuid = ap_get_uuid(mDataFolder + "ap_uuid.txt", theServer);
	mClient = new APClient(aUuid, AP_GAME_NAME, theServer, mCertFile);
	mState = AP_SOCKET_CONNECTING;

	mClient->set_socket_error_handler([this](const std::string& theError)
	{
		mLastError = theError;
	});

	mClient->set_socket_disconnected_handler([this]()
	{
		// apclientpp reconnects on its own; we just have to log in again after the next RoomInfo.
		mState = AP_SOCKET_CONNECTING;
	});

	mClient->set_room_info_handler([this]()
	{
		mState = AP_SLOT_CONNECTING;
		mClient->ConnectSlot(mSlot, mPassword, AP_ITEMS_HANDLING);
	});

	mClient->set_slot_connected_handler([this](const nlohmann::json&)
	{
		mState = AP_SLOT_CONNECTED;
		mLastError.clear();
	});

	mClient->set_slot_refused_handler([this](const std::list<std::string>& theErrors)
	{
		mState = AP_SLOT_REFUSED;
		mLastError.clear();
		for (const std::string& anError : theErrors)
		{
			if (!mLastError.empty())
				mLastError += ", ";
			mLastError += anError;
		}
	});
}

void APBridge::Disconnect()
{
	delete mClient;
	mClient = NULL;
	mState = AP_DISCONNECTED;
}

void APBridge::Update()
{
	if (mClient != NULL)
		mClient->poll();
}
