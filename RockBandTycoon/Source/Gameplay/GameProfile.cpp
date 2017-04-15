#include "PCH.hpp"

#include "GameProfile.hpp"
#include <fstream>
#include "Components/Names.hpp"
#include "Components/Resources.hpp"
#include "Views/DialogView.hpp"

using namespace vh;

const uint32_t GameProfile::VERSION_TAG = 0x31bca0bd;

static const int32_t START_MONEY = 1000;
static const float START_REP = 0.0f;
static const float START_POP = 0.0f;

GameProfile::GameProfile(int slot, const char* name /* = "" */)
    : mBandName(name)
    , mMoney(0)
    , mDay(0)
    , mSkill(0)
    , mReputation(0)
    , mPopularity(0)
    , mQuality(0)
    , mSlot(slot)
    , mShowMOTG(false)
{
    mGuitarist = BandMember(eBandSlot::Guitar, "", Item(), Looks());
    mBassist = BandMember(eBandSlot::Bass, "", Item(), Looks());
    mDrummer  = BandMember(eBandSlot::Drums, "", Item(), Looks());
}

bool GameProfile::GetProfilePath(std::string& outPath, int slot)
{
    const char* path = SDL_GetPrefPath(ORG_NAME, APP_NAME);
    if (!path)
    {
        LOG(ERROR) << "Could not get preferences folder" << SDL_GetError();
        return false;
    }

    outPath = path;

    const char* filename;
    switch (slot)
    {
    case 1: filename = "profile1.dat"; break;
    case 2: filename = "profile2.dat"; break;
    case 3: filename = "profile3.dat"; break;
    default:
        LOG(ERROR) << "Invalid slot: " << slot;
        return false;
    }

    outPath.append(filename);

    return true;
}

void GameProfile::AddMember(const BandMember& member)
{
    switch (member.GetType())
    {
    case eBandSlot::Bass:
        SetBassist(member);
        break;
    case eBandSlot::Guitar:
        SetGuitarist(member);
        break;
    case eBandSlot::Drums:
        SetDrummer(member);
        break;
    default:
        LOG(ERROR) << "Unsupported member type: " << member.GetType();
    }
}

const BandMember& GameProfile::GetBandMember(eBandSlot::Type type) const
{
    switch (type)
    {
    case eBandSlot::Bass:
        return mBassist;
    case eBandSlot::Guitar:
        return mGuitarist;
    case eBandSlot::Drums:
        return mDrummer;
    default:
        LOG(ERROR) << "Unsupported member type: " << type;
        return mGuitarist;
    }
}

void GameProfile::SetItem(eBandSlot::Type type, const Item& item)
{
    BandMember mem = GetBandMember(type);
    AddMember(BandMember(type, mem.GetName().c_str(), item, mem.GetLooks()));
}

GameProfile* GameProfile::NewProfile(int slot, const std::string& name)
{
    GameProfile* profile = new GameProfile(slot, name.c_str());
    profile->SetShop(Shop::Generate());
    profile->SetMoney(START_MONEY);
    profile->SetPopularity(START_POP);
    profile->SetReputation(START_REP);

    Names* names = App::Get<Names>();
    Resources* res = App::Get<Resources>();
    CHECK(names);
    CHECK(res);

    std::vector<HireItem> guitarHires;
    guitarHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Guitar), 0, 0.0f, 0.0f));
    guitarHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Guitar), 50, 0.0f, 0.0f));
    guitarHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Guitar), 100, 20.0f, 0.0f));
    guitarHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Guitar), 100, 0.0f, -40.0f));
    profile->SetGuitarHires(guitarHires);

    std::vector<HireItem> bassHires;
    bassHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Bass), 0, 0.0f, 0.0f));
    bassHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Bass), 50, 20.0f, 0.0f));
    bassHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Bass), 100, 0.0f, -10.0f));
    profile->SetBassHires(bassHires);

    std::vector<HireItem> drumHires;
    drumHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Drums), 0, 40.0f, 0.0f));
    drumHires.push_back(HireItem(names->GetRandomMaleName(), res->GetRandomLooks(eBandSlot::Drums), 40, -20.0f, 0.0f));
    profile->SetDrumHires(drumHires);

    profile->StartNextDay(-1);

    return profile;
}

bool GameProfile::WithdrawMoney(int32_t amount)
{
    if (amount < mMoney)
    {
        SetMoney(mMoney - amount);
        return true;
    }
    else
    {
        return false;
    }
}

void GameProfile::StartNextDay(int eventId)
{
    if (eventId >= 0 && eventId < mEvents.size())
    {
        const EventItem& event = mEvents[eventId];
        SetMoney(mMoney + event.GetMoney());
        SetReputation(mReputation + event.GetReputation());
        SetPopularity(mPopularity + event.GetPopularity());
        SetSkill(mSkill + event.GetSkill());
    }
    SetEvents(GenerateEvents());
    SetDay(mDay + 1);

    mShowMOTG = true;

    // TODO: update shop and hires
    // TODO: check for game over condition
}

std::vector<EventItem> GameProfile::GenerateEvents()
{
    std::vector<EventItem> events;
    events.push_back(EventItem("Rehearsal", "No concerts for today, let's just focus on practice",
        -10, 0.0f, -5.0f, 20.0f));
    return events;
}

/*
    Write to binary stream. Default implementation.
*/
template<typename T>
void Write(std::ostream& s, const T& val)
{
    s.write((char*) &val, sizeof(T));
}

/*
    Read from binary stream. Default implementation.
*/
template<typename T>
void Read(std::istream& s, T& val)
{
    s.read((char*) &val, sizeof(T));
}

/*
    Write: std::vector
*/
template<typename T>
void Write(std::ostream& file, const std::vector<T>& obj)
{
    Write(file, obj.size());
    for (const T& item : obj)
    {
        Write(file, item);
    }
}

template<typename T>
void Read(std::istream& file, std::vector<T>& obj)
{
    size_t size;
    Read(file, size);
    obj = std::vector<T>(size);
    for (size_t idx = 0; idx < size; ++idx)
    {
        Read(file, obj[idx]);
    }
}

/*
    Write: std::string
*/
template<> void Write(std::ostream& file, const std::string& str)
{
    std::string::size_type sz = str.size();
    file.write((char*) &sz, sizeof(std::string::size_type));
    file.write(str.c_str(), str.size());
}

/*
    Read: std::string
*/
template<> void Read(std::istream& file, std::string& outStr)
{
    std::string::size_type sz;
    file.read((char*) &sz, sizeof(std::string::size_type));
    char* buf = new char[sz];
    file.read(buf, sz);
    outStr.assign(buf, sz);
}

/*
    Write: Looks
*/
template<> void Write(std::ostream& file, const Looks& looks)
{
    Write(file, looks.GetHair());
    Write(file, looks.GetFace());
    Write(file, looks.GetBody());
    Write(file, looks.GetHands());
    Write(file, looks.GetLegs());
}

/*
    Read: Looks
*/
template<> void Read(std::istream& file, Looks& looks)
{
    std::string hair, face, body, hands, legs;
    Read(file, hair);
    Read(file, face);
    Read(file, body);
    Read(file, hands);
    Read(file, legs);
    looks = Looks(hair, face, body, hands, legs);
}

/*
    Write: Item
*/
template<> void Write(std::ostream& file, const Item& item)
{
    Write(file, item.GetName());
    Write(file, item.GetImg());
}

/*
    Read: Item
*/
template<> void Read(std::istream& file, Item& item)
{
    std::string name, img;
    Read(file, name);
    Read(file, img);
    item = Item(name, img);
}

/*
    Write: BandMember
*/
template<> void Write(std::ostream& file, const BandMember& bm)
{
    Write(file, bm.GetName());
    Write(file, bm.GetType());
    Write(file, bm.GetItem());
    Write(file, bm.GetLooks());
}

/*
    Read: BandMember
*/
template<> void Read(std::istream& file, BandMember& bm)
{
    std::string name;
    eBandSlot::Type type;
    Read(file, name);
    Read(file, type);
    Item item; Read(file, item);
    Looks looks; Read(file, looks);
    bm = BandMember(type, name.c_str(), item, looks);
}

/*
    Write: ShopItem
*/
template<> void Write(std::ostream& file, const ShopItem& obj)
{
    Write(file, obj.GetType());
    Write(file, obj.GetName());
    Write(file, obj.GetImg());
    Write(file, obj.GetCost());
    Write(file, obj.GetReputation());
    Write(file, obj.GetPopularity());
    Write(file, obj.GetQuality());
}

/*
    Read: ShopItem
*/
template<> void Read(std::istream& file, ShopItem& obj)
{
    eBandSlot::Type type;
    Read(file, type);
    std::string name, img;
    Read(file, name);
    Read(file, img);
    int32_t cost;
    Read(file, cost);
    float rep, pop, qual;
    Read(file, rep);
    Read(file, pop);
    Read(file, qual);
    obj = ShopItem(type, name, img, cost, rep, pop, qual);
}

/*
    Write: Shop
*/
template<> void Write(std::ostream& file, const Shop& obj)
{
    Write(file, obj.GetGuitars());
    Write(file, obj.GetBassGuitars());
    Write(file, obj.GetDrums());
}

/*
    Read: Shop
*/
template<> void Read(std::istream& file, Shop& obj)
{
    std::vector<ShopItem> g, b, d;
    Read(file, g);
    Read(file, b);
    Read(file, d);
    obj = Shop(g, b, d);
}

/*
    Write: HireItem
*/
template<> void Write(std::ostream& file, const HireItem& obj)
{
    Write(file, obj.GetName());
    Write(file, obj.GetLooks());
    Write(file, obj.GetCost());
    Write(file, obj.GetReputation());
    Write(file, obj.GetPopularity());
}

/*
    Read: HireItem
*/
template<> void Read(std::istream& file, HireItem& obj)
{
    std::string name;
    Looks looks;
    int32_t cost;
    Read(file, name);
    Read(file, looks);
    Read(file, cost);
    float rep, pop;
    Read(file, rep);
    Read(file, pop);
    obj = HireItem(name, looks, cost, rep, pop);
}

/*
    Write: EventItem
*/
template<> void Write(std::ostream& file, const EventItem& obj)
{
    Write(file, obj.GetName());
    Write(file, obj.GetDescription());
    Write(file, obj.GetMoney());
    Write(file, obj.GetReputation());
    Write(file, obj.GetPopularity());
    Write(file, obj.GetSkill());
}

/*
    Read: EventItem
*/
template<> void Read(std::istream& file, EventItem& obj)
{
    std::string name, description;
    Read(file, name);
    Read(file, description);
    int32_t money;
    Read(file, money);
    float rep, pop, skill;
    Read(file, rep);
    Read(file, pop);
    Read(file, skill);
    obj = EventItem(name, description, money, rep, pop, skill);
}

bool GameProfile::Save() const
{
    std::string fullPath;
    if (!GetProfilePath(fullPath, mSlot)) return false;

    std::fstream file(fullPath.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!file.is_open())
    {
        LOG(ERROR) << "Could not open profile file '" << fullPath << "'";
        return false;
    }

    // right out all profile
    Write(file, VERSION_TAG);

    Write(file, mBandName);
    Write(file, mDay);
    Write(file, mMoney);
    Write(file, mPopularity);
    Write(file, mQuality);
    Write(file, mReputation);
    Write(file, mSkill);

    Write(file, mGuitarist);
    Write(file, mBassist);
    Write(file, mDrummer);

    Write(file, mShop);

    Write(file, mGuitarHires);
    Write(file, mBassHires);
    Write(file, mDrumHires);

    Write(file, mEvents);
    Write(file, mShowMOTG);

    file.close();

    return true;
}

bool GameProfile::Load()
{
    std::string fullPath;
    if (!GetProfilePath(fullPath, mSlot)) return false;

    std::fstream file(fullPath.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
    {
        LOG(ERROR) << "Could not open profile file '" << fullPath << "'";
        return false;
    }

    uint32_t tag;
    Read(file, tag);
    if (tag != VERSION_TAG)
    {
        LOG(ERROR) << "Version tag mismatch: " << tag;
        file.close();
        return false;
    }

    Read(file, mBandName);
    Read(file, mDay);
    Read(file, mMoney);
    Read(file, mPopularity);
    Read(file, mQuality);
    Read(file, mReputation);
    Read(file, mSkill);

    Read(file, mGuitarist); CHECK(mGuitarist.GetType() == eBandSlot::Guitar);
    Read(file, mBassist); CHECK(mBassist.GetType() == eBandSlot::Bass);
    Read(file, mDrummer); CHECK(mDrummer.GetType() == eBandSlot::Drums);

    Read(file, mShop);

    Read(file, mGuitarHires);
    Read(file, mBassHires);
    Read(file, mDrumHires);

    Read(file, mEvents);
    Read(file, mShowMOTG);

    file.close();

    return true;
}
