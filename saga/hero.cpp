// Copyright 2013 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "main.h"

static unsigned short	abilities[7];
static int				class_hd[] = {10, 6, 6, 8, 10, 4, 10};
static int				ClassSkillPoints[] = {2, 6, 4, 5, 3, 2, 0};
static int				SkillsRang[] = {0, 5, 7, 9, 11, 13};
static int				DualWeaponMasteryPenalty[] = {-10, -5, -2, 0};
static int				SkillsAttackBonus[] = {-5, 0, 1, 1, 2, 2, 3};
static int				skills_damage_bonus[] = {0, 0, 0, 2, 2, 4, 4};
static int				experience_table[] = {0, 1000, 3000, 6000, 10000, 15000, 21000, 28000, 36000, 45000, 55000,
66000, 78000, 91000, 105000, 120000, 136000, 153000, 171000, 190000, 0
};

static const wchar_t* human_male_prefics[] = {L"ви", L"ку", L"кор", L"вар", L"со", L"эне"};
static const wchar_t* human_male[] = {L"люк", L"мейс", L"павел", L"александр", L"андрей", L"юрий",
L"боб", L"илья", L"игорь", L"олег", L"егор", L"макс", L"джон", L"брэндон", L"льюис", L"оливер"};
static const wchar_t* human_female[] = {L"изабель", L"лора", L"хейли", L"елена", L"бренда", L"алиса",
L"рута", L"юлия", L"вероника", L"сильвия", L"полли", L"евгения", L"ирина", L"оливия"};
static const wchar_t* human_male_postfics[] = {L"то", L"рах", L"бейн", L"кен", L"ло", L"нел"};
static const wchar_t* human_family_end[] = {L"вокер", L"смис", L"готтен", L"вуд", L"фоддер"};
static const wchar_t* human_family_begin[] = {L"скай", L"блек", L"вайт", L"кэнон", L"скин", L"грин", L"колд", L"файр"};

namespace setting
{
	const int			powers = 9;
}

struct feat_weight
{
	int	feat;
	int	weight;
	//
	static int compare(const void* c1, const void* c2, void* param)
	{
		return ((feat_weight*)c2)->weight - ((feat_weight*)c1)->weight;
	}
};

int d100()
{
	return xrand()%100;
}

//static bool isequipment(int token)
//{
//	switch(token)
//	{
//	case Melee:
//	case Missile:
//	case Wear:
//	case Slot1:
//	case Slot2:
//	case Slot3:
//	case Slot4:
//	case Slot5:
//	case Slot6:
//	case Slot7:
//	case Slot8:
//		return true;
//	default:
//		return false;
//	}
//}

static int best4d6compare(const void *c1, const void *c2, void* param)
{
	return *((char*)c2)-*((char*)c1);
}

static unsigned char best4d6()
{
	char d[4];
	d[0] = (xrand()%6)+1;
	d[1] = (xrand()%6)+1;
	d[2] = (xrand()%6)+1;
	d[3] = (xrand()%6)+1;
	xsort(&d, sizeof(d), 1, &best4d6compare, 0);
	return d[0]+d[1]+d[2];
}

static int gbitscount(unsigned i)
{
	int s = 0;
	for(int j = 0; j<32; j++)
		if((i&(1<<j))!=0)
			s++;
	return s;
}

int unit::get(int i) const
{
	static int StealthSizeBonus[] = {10, 5, 0, -5, -10, -20};
	static int StealthRefBonus[] = {2, 1, 0, -1, -2, -4};
	int n, m;
	switch(i)
	{
	case Reflexes:
		return 10
			+ gbonusref(false)
			+ (attr[Class]==Jedi ? 1 : 0)
			+ (attr[Class]==Soldier ? 1 : 0)
			+ (attr[Class]==Scoundrel ? 2 : 0)
			+ (attr[Class]==Scout ? 2 : 0)
			+ (attr[Class]==BountyHunter ? 4 : 0)
			+ get(ImprovedDefences)
			+ StealthRefBonus[get(Size)-Tiny]
			+ get(LightingReflexes)*2
			+ get(NaturalArmour)
			+ get(Dodge);
	case Fortitude:
		return 10
			+ get(HeroicLevel)
			+ (attr[Class]==Jedi ? 1 : 0)
			+ (attr[Class]==Scout ? 1 : 0)
			+ (attr[Class]==Soldier ? 2 : 0)
			+ (attr[Class]==BountyHunter ? 2 : 0)
			+ get(ImprovedDefences)
			+ get(GreatFortitude)*2
			+ gbonus(Con)
			+ gbonusfort();
	case Will:
		return 10
			+ get(HeroicLevel)
			+ (attr[Class]==Jedi ? 1 : 0)
			+ (attr[Class]==Noble ? 2 : 0)
			+ (attr[Class]==Scoundrel ? 1 : 0)
			+ get(ImprovedDefences)
			+ get(IronWill)*2
			+ gbonus(Wis);
	case ReflexesFlatFooted:
		return 10
			+ gbonusref(true)
			+ (Class==Jedi ? 1 : 0)
			+ (Class==Scoundrel ? 2 : 0)
			+ (Class==Scout ? 2 : 0)
			+ get(NaturalArmour)
			+ StealthRefBonus[get(Size)-Tiny];
	case BaB:
		return ((attr[Noble]+attr[Scoundrel]+attr[Scout])*3/4) + attr[Soldier] + attr[Jedi];
	case HitsTotal:
		return get(HitDice)*3
			+ get(HeroicLevel)*(gbonus(Con)+get(Toughness))
			+ attr[HitsRolled];
	case HitDice:
		return class_hd[attr[Class]-Jedi];
	case FeatsAllowed:
		n = get(HeroicLevel);
		return n/3 + 1 + n/2 + (attr[Race]==Human ? 1 : 0);
	case Feats:
		return gattrc(AcrobaticStrike, WhirlwindAttack, false)
			+ gattrc(Acrobatic, UseForce, false) - gattrc(Acrobatic, UseForce, true)
			+ gattrcmax(SimpleWeapons, LightSabers, 2);
	case FeatsPoints:
		return get(FeatsAllowed)-get(Feats);
	case TalentsAllowed:
		return (attr[Jedi]+1)/2 + (attr[Soldier]+1)/2 + (attr[Noble]+1)/2 + (attr[Scoundrel]+1)/2 + (attr[Scout]+1)/2;
	case Talents:
		return gattrc(AdeptNegotiator, Visions, false)
			+ gattrc(SimpleWeapons, LightSabers, false, 2);
	case TalentsPoints:
		return get(TalentsAllowed)-get(Talents);
	case SkillsAllowed:
		n = 0;
		for(int s = Jedi; s<=Soldier; s++)
		{
			if(attr[s])
				n = imax(ClassSkillPoints[s-Jedi], n);
		}
		return imax(n
			+ attr[Int]/2 - 5
			+ (attr[Race]==Human ? 1 : 0)
			+ get(SkillTraining), 0);
	case Skills:
		n = 0;
		for(int s = FirstSkill; s<=LastSkill; s++)
		{
			if(attr[s])
				n++;
		}
		return n;
	case SkillsPoints:
		return get(SkillsAllowed)-get(Skills);
	case ForcePowersAllowed:
		return get(ForceTraining)*(1+attr[Wis]/2-5);
	case ForcePowers:
		return gattrc(BattleStrike, VitalTransfer, false);
	case ForcePowersPoints:
		return get(ForcePowersAllowed)-get(ForcePowers);
	case ForcePoints:
		return attr[ForcePoints];
	case ForcePointsByLevel:
		return 5 + get(HeroicLevel)/2 + (get(ForceBoon) ? 3 : 0);
	case DestinyPoints:
		return attr[DestinyPoints];
	case Size:
		switch(Race)
		{
		case Ewoks:
			return Small;
		default:
			return Medium;
		}
		return 0;
	case DamageTreshold:
		return get(Fortitude) + get(ImprovedDamageThreshold)*5;
	case HeroicLevel:
		return attr[Jedi] + attr[Noble] + attr[Scoundrel] + attr[Scout] + attr[Soldier]
			+ attr[AcePilot] + attr[CrimeLord] + attr[JediKnight] + attr[JediMaster];
	case Speed:
		switch(Race)
		{
		case Ewoks:
			n = 4;
			break;
		default:
			n = 6;
			break;
		}
		m = 0;
		if(wears.armour.isvalid())
		{
			switch(wears.armour.get(Class))
			{
			case ArmourHeavy:
			case ArmourMedium:
				n = (n*3)/4;
				m = 1;
				break;
			default:
				break;
			}
		}
		if(m==0 && get(LongStride))
			n += 2;
		return n;
	case Languages:
		return gbitscount(attr[Languages]);
	case LanguagesAllowed:
		return imax(0, gbonus(Int) + get(Linguist)*(1+gbonus(Int)));
	case LanguagesPoints:
		return get(LanguagesAllowed)-get(Languages);
	case StrBonus:
	case DexBonus:
	case ConBonus:
	case IntBonus:
	case WisBonus:
	case ChaBonus:
		return get(i-StrBonus+Str)/2-5;
	case Str:
		n = attr[Str];
		if(wears.armour.get(Class)==CorelianPowerSuit)
			n += 2;
		return n;
	case EncumbranceWeight:
		m = get(Str);
		return (5*m*m)/2;
	case EquipmentWeight:
		m = wears.armour.get(Weight)
			+ wears.melee.get(Weight)
			+ wears.second.get(Weight)
			+ wears.ranged.get(Weight);
		for(unsigned k = 0; k<sizeof(posses)/sizeof(posses[0]); k++)
			m += posses[k].get(Weight);
		return m;
		// Race ability
	case Bellow:
		return attr[i] + ((attr[Race]==Ithorians) ? 1 : 0);
	case BreathUnderwater:
		return attr[i] + ((attr[Race]==MonCalamari || attr[Race]==Quarren) ? 1 : 0);
	case Darkvision:
		return attr[i] + ((attr[Race]==Sullustans || attr[Race]==Txrandoshans) ? 1 : 0);
	case Deceptive:
		return attr[i] + ((attr[Race]==Twileks) ? 1 : 0);
	case ExpertClimber:
		return attr[i] + ((attr[Race]==Sullustans) ? 1 : 0);
	case ExpertSwimmer:
		return attr[i] + ((attr[Race]==Gungans || attr[Race]==MonCalamari || attr[Race]==Quarren) ? 1 : 0);
	case ExpertPilot:
		return attr[i] + ((attr[Race]==Duros) ? 1 : 0);
	case ExtraordinaryRecuperation:
		return attr[i] + ((attr[Race]==Wookie) ? 1 : 0);
	case GreatFortitude:
		return attr[i] + ((attr[Race]==Gammorean || attr[Race]==Twileks) ? 1 : 0);
	case HeightenAwareness:
		return attr[i] + ((attr[Race]==Rodians || attr[Race]==Sullustans || attr[Race]==Zabrak) ? 1 : 0);
	case HoldBreath:
		return attr[i] + ((attr[Race]==Gungans) ? 1 : 0);
	case IntuitiveInitiative:
		return attr[i] + ((attr[Race]==Cerean) ? 1 : 0);
	case IronWill:
		return attr[i] + ((attr[Race]==Bothan || attr[Race]==Ithorians) ? 1 : 0);
	case KeenForceSence:
		return attr[i] + ((attr[Race]==KelDor) ? 1 : 0);
	case LightingReflexes:
		return attr[i] + ((attr[Race]==Gungans) ? 1 : 0);
	case LimbRegeneration:
		return attr[i] + ((attr[Race]==Txrandoshans) ? 1 : 0);
	case LowlightVision:
		return ((attr[Race]==Gungans || attr[Race]==KelDor || attr[Race]==MonCalamari
			|| attr[Race]==Quarren || attr[Race]==Rodians || attr[Race]==Twileks) ? 1 : 0);
	case NaturalArmour:
		return attr[i] + ((attr[Race]==Txrandoshans) ? 1 : 0);
	case Primitive:
		return attr[i] + ((attr[Race]==Ewoks || attr[Race]==Gammorean) ? 1 : 0);
	case Rage:
		return attr[i] + ((attr[Race]==Wookie) ? 1 : 0);
	case Scent:
		return attr[i] + ((attr[Race]==Ewoks) ? 1 : 0);
	case Sneaky:
		return attr[i] + ((attr[Race]==Ewoks) ? 1 : 0);
	case SurvivalInstinct:
		return attr[i] + ((attr[Race]==Ithorians) ? 1 : 0);
	case ImprovedDamageThreshold:
		return attr[i] + ((attr[Race]==Gammorean) ? 1 : 0);
	case Toughness:
		return attr[i] + ((attr[Race]==Txrandoshans) ? 1 : 0);
		// Languages
	case Basic:
		return 1;
	case Bothese:
		return (attr[Race]==Bothan) ? 1 : ((attr[Languages]&(1<<(Bothese-Basic)))!=0 ? 1 : 0);
	case CereanLanguage:
		return (attr[Race]==Cerean) ? 1 : ((attr[Languages]&(1<<(CereanLanguage-Basic)))!=0 ? 1 : 0);
	case Durese:
		return (attr[Race]==Duros) ? 1 : ((attr[Languages]&(1<<(Durese-Basic)))!=0 ? 1 : 0);
	case Ewokese:
		return (attr[Race]==Ewoks) ? 1 : ((attr[Languages]&(1<<(Ewokese-Basic)))!=0 ? 1 : 0);
	case GammoreanLanguage:
		return (attr[Race]==Gammorean) ? 1 : ((attr[Languages]&(1<<(GammoreanLanguage-Basic)))!=0 ? 1 : 0);
	case Gunganese:
		return (attr[Race]==Gungans) ? 1 : ((attr[Languages]&(1<<(Gunganese-Basic)))!=0 ? 1 : 0);
	case Ithorese:
		return (attr[Race]==Ithorians) ? 1 : ((attr[Languages]&(1<<(Ithorese-Basic)))!=0 ? 1 : 0);
	case KelDorLanguage:
		return (attr[Race]==KelDor) ? 1 : ((attr[Languages]&(1<<(KelDorLanguage-Basic)))!=0 ? 1 : 0);
	case MonCalamarian:
		return (attr[Race]==MonCalamari) ? 1 : ((attr[Languages]&(1<<(MonCalamarian-Basic)))!=0 ? 1 : 0);
	case Quarreness:
		return (attr[Race]==Quarren) ? 1 : ((attr[Languages]&(1<<(Quarreness-Basic)))!=0 ? 1 : 0);
	case Rodese:
		return (attr[Race]==Rodians) ? 1 : ((attr[Languages]&(1<<(Rodese-Basic)))!=0 ? 1 : 0);
	case Shyriiwook:
		return (attr[Race]==Wookie) ? 1 : ((attr[Languages]&(1<<(Shyriiwook-Basic)))!=0 ? 1 : 0);
	case Sullustese:
		return (attr[Race]==Sullustans) ? 1 : ((attr[Languages]&(1<<(Sullustese-Basic)))!=0 ? 1 : 0);
	case ZabrakLanguage:
		return (attr[Race]==Zabrak) ? 1 : ((attr[Languages]&(1<<(ZabrakLanguage-Basic)))!=0 ? 1 : 0);
		// Свободный фокус на навык
	case ForceSensitivity:
		return attr[i] + ((attr[Class]==Jedi) ? 1 : 0);
	case LightSabers:
		return attr[i] + ((attr[Class]==Jedi) ? 1 : 0);
	case SimpleWeapons:
		return attr[i] + ((attr[Class]==Jedi || attr[Class]==Noble || attr[Class]==Scoundrel || attr[Class]==Scout || attr[Class]==Soldier) ? 1 : 0);
	case Linguist:
		return attr[i] + ((attr[Class]==Noble && attr[::Int]>=13) ? 1 : 0);
	case Pistols:
		return attr[i] + ((attr[Class]==Noble || attr[Class]==Scoundrel || attr[Class]==Scout || attr[Class]==Soldier) ? 1-get(Primitive) : 0);
	case PointBlankShoot:
		return attr[i] + ((attr[Class]==Scoundrel) ? 1 : 0);
	case ShakeItOff:
		return attr[i] + ((attr[Class]==Scout && allow(i, 1)) ? 1 : 0);
	case Rifles:
		return attr[i] + ((attr[Class]==Scout || attr[Class]==Soldier) ? 1-get(Primitive) : 0);
	case ArmourProficience:
		return attr[i] + ((attr[Class]==Soldier) ? 2 : 0);
		// специальные способности престижных классов
	case VehicleDodge:
		return attr[AcePilot]/2;
	case FamiliarFoe:
		return attr[BountyHunter]/2;
		// skills
	case Climb:
	case Jump:
	case Swim:
		return get(HeroicLevel)/2
			+ gbonus(Str)
			+ SkillsRang[attr[i]];
	case Acrobatic:
	case Pilot:
	case Ride:
		return get(HeroicLevel)/2
			+ gbonus(Dex)
			+ SkillsRang[attr[i]];
	case Initiative:
		return get(HeroicLevel)/2
			+ gbonus(Dex)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==Cerean) ? 1 : 0)) : 0];
	case Stealth:
		return get(HeroicLevel)/2
			+ gbonus(Dex)
			+ StealthSizeBonus[get(Size)-Tiny]
			+ SkillsRang[attr[i]];
	case Endurance:
		return get(HeroicLevel)/2
			+ gbonus(Con)
			+ SkillsRang[attr[i]];
	case Buerocracy:
	case GalacticLore:
	case PhysicalSciences:
	case SocialSciences:
	case Tactics:
	case Technology:
	case Mechanics:
	case UseComputer:
		return get(HeroicLevel)/2
			+ gbonus(Int)
			+ SkillsRang[attr[i]];
	case LifeSciences:
		return get(HeroicLevel)/2
			+ gbonus(Int)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==Ithorians) ? 1 : 0)) : 0];
	case TreatInjury:
		return get(HeroicLevel)/2
			+ gbonus(Wis)
			+ SkillsRang[attr[i]];
	case Perception:
		return get(HeroicLevel)/2
			+ gbonus(Wis)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==MonCalamari) ? 1 : 0)) : 0];
	case Survival:
		return get(HeroicLevel)/2
			+ gbonus(Wis)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==Ewoks || Race==Rodians) ? 1 : 0)) : 0];
	case UseForce:
	case Deception:
		return get(HeroicLevel)/2
			+ gbonus(Cha)
			+ SkillsRang[attr[i]];
	case Persuasion:
		return get(HeroicLevel)/2
			+ gbonus(Cha)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==Quarren) ? 1 : 0)) : 0];
	case GatherInformation:
		return get(HeroicLevel)/2
			+ gbonus(Cha)
			+ SkillsRang[attr[i] ? (attr[i]+((Race==Bothan) ? 1 : 0)) : 0];
	case Gender:
		if(attr[i])
			return Female;
		return Male;
	case EquipmentCost:
		i = 0;
		for(auto e : posses)
			i += e.get(Credits);
		i += wears.armour.get(Credits);
		i += wears.melee.get(Credits);
		i += wears.second.get(Credits);
		i += wears.ranged.get(Credits);
		return i;
	default:
		if(i<=DarkSideScore)
			return attr[i];
		else if(i>=Basic && i<=ZabrakLanguage)
			return (attr[Languages]&(1<<(i-Basic)))!=0;
		return 0;
	}
}

bool unit::get(int token, wchar_t* v) const
{
	switch(token)
	{
	case Name:
		if(name)
			szcpy(v, name);
		else
			szcpy(v, gstr(NewCharacter));
		break;
	case Body:
		wears.armour.get(Name, v);
		get(wears.armour, v);
		v[0] = chupper(v[0]);
		break;
	case Melee:
		wears.melee.get(Name, v);
		get(wears.melee, v);
		v[0] = chupper(v[0]);
		break;
	case Missile:
		wears.ranged.get(Name, v);
		get(wears.ranged, v);
		v[0] = chupper(v[0]);
		break;
	default:
		return false;
	}
	return true;
}

const item* unit::gitem(int token) const
{
	switch(token)
	{
	case Armour:
		return &wears.armour;
	case Melee:
		return &wears.melee;
	case Missile:
		return &wears.ranged;
	case EmpthySlot:
		for(unsigned i = 0; i<sizeof(posses)/sizeof(posses[0]); i++)
		{
			if(!posses[i].isvalid())
				return &posses[i];
		}
		return 0;
	case Slot1:
	case Slot2:
	case Slot3:
	case Slot4:
	case Slot5:
	case Slot6:
	case Slot7:
	case Slot8:
		return &posses[token-Slot1];
	default:
		return 0;
	}
}

bool unit::set(int i, const int v)
{
	item* p;
	switch(i)
	{
	case Abilities:
		if(v==Generate)
		{
			abilities[0] = best4d6();
			abilities[1] = best4d6();
			abilities[2] = best4d6();
			abilities[3] = best4d6();
			abilities[4] = best4d6();
			abilities[5] = best4d6();
			xsort(abilities, 6, sizeof(abilities[0]), best4d6compare, 0);
		}
		else
		{
			bool woman = get(Gender)==Female;
			switch(v)
			{
			case Jedi:
				if(woman)
				{
					attr[Cha] = abilities[0];
					attr[Wis] = abilities[1];
				}
				else
				{
					attr[Wis] = abilities[0];
					attr[Cha] = abilities[1];
				}
				attr[Dex] = abilities[3];
				attr[Str] = abilities[2];
				attr[Int] = abilities[4];
				attr[Con] = abilities[5];
				break;
			case Noble:
				if(woman)
				{
					attr[Cha] = abilities[0];
					attr[Int] = abilities[1];
					attr[Wis] = abilities[2];
				}
				else
				{
					attr[Int] = abilities[0];
					attr[Wis] = abilities[1];
					attr[Cha] = abilities[2];
				}
				attr[Dex] = abilities[3];
				attr[Con] = abilities[4];
				attr[Str] = abilities[5];
				break;
			case Scoundrel:
				attr[Dex] = abilities[0];
				attr[Cha] = abilities[2];
				attr[Str] = abilities[1];
				attr[Con] = abilities[3];
				attr[Int] = abilities[4];
				attr[Wis] = abilities[5];
				break;
			case Scout:
				attr[Wis] = abilities[0];
				attr[Dex] = abilities[1];
				attr[Str] = abilities[2];
				attr[Con] = abilities[3];
				attr[Int] = abilities[4];
				attr[Cha] = abilities[5];
				break;
			default:
				attr[Str] = abilities[0];
				attr[Dex] = abilities[1];
				attr[Con] = abilities[2];
				attr[Int] = abilities[3];
				attr[Wis] = abilities[4];
				attr[Cha] = abilities[5];
				break;
			}
			// apply race modifier
			switch(attr[Race])
			{
			case Bothan:
				attr[Dex] += 2;
				attr[Con] -= 2;
				break;
			case Cerean:
				attr[Int] += 2;
				attr[Wis] += 2;
				attr[Dex] -= 2;
				break;
			case Duros:
				attr[Dex] += 2;
				attr[Int] += 2;
				attr[Con] -= 2;
				break;
			case Ewoks:
				attr[Dex] += 2;
				attr[Str] -= 2;
				break;
			case Gammorean:
				attr[Str] += 2;
				attr[Dex] -= 2;
				attr[Int] -= 2;
				break;
			case Gungans:
				attr[Dex] += 2;
				attr[Int] -= 2;
				attr[Cha] -= 2;
				break;
			case Ithorians:
				attr[Wis] += 2;
				attr[Cha] += 2;
				attr[Dex] -= 2;
				break;
			case KelDor:
				attr[Dex] += 2;
				attr[Wis] += 2;
				attr[Con] -= 2;
				break;
			case MonCalamari:
				attr[Int] += 2;
				attr[Wis] += 2;
				attr[Con] -= 2;
				break;
			case Quarren:
				attr[Con] += 2;
				attr[Wis] -= 2;
				attr[Cha] -= 2;
				break;
			case Rodians:
				attr[Dex] += 2;
				attr[Wis] -= 2;
				attr[Cha] -= 2;
				break;
			case Sullustans:
				attr[Dex] += 2;
				attr[Con] -= 2;
				break;
			case Txrandoshans:
				attr[Str] += 2;
				attr[Dex] -= 2;
				break;
			case Twileks:
				attr[Cha] += 2;
				attr[Wis] -= 2;
				break;
			case Wookie:
				attr[Str] += 4;
				attr[Con] += 2;
				attr[Dex] -= 2;
				attr[Wis] -= 2;
				attr[Cha] -= 2;
				break;
			default:
				break;
			}
		}
		break;
	case Class:
		if(get(HeroicLevel)>0)
			attr[HitsRolled] += (xrand()%(class_hd[v-Jedi]))+1;
		else
		{
			attr[Class] = v;
			attr[HitsRolled] = 0;
		}
		attr[v]++;
		break;
	case Gender:
		if(v==Female)
			attr[Gender] = 1;
		else
			attr[Gender] = 0;
		break;
	case ForcePoints:
		attr[ForcePoints] = get(ForcePointsByLevel);
		break;
	case Skills:
		if(v==Generate)
		{
			int m = get(SkillsAllowed);
			if(get(Skills)<m)
			{
				int	count = 0;
				feat_weight	weights[UseForce-Acrobatic+1];
				for(int i = Acrobatic; i<=UseForce; i++)
				{
					if(!attr[i] && allow(i, 1))
					{
						weights[count].feat = i;
						weights[count].weight = get(i)*2 + xrand()%4;
						count++;
					}
				}
				xsort(weights, count, sizeof(feat_weight), feat_weight::compare, 0);
				for(int i = 0; i<count && get(Skills)<m; i++)
					set(weights[i].feat, Increment);
			}
		}
		break;
	case Talents:
		if(v==Generate)
		{
			while(get(Talents)<get(TalentsAllowed))
			{
				int weights[LightSabers-AdeptNegotiator+1];
				int	count = get(weights, Talents, false);
				if(!count)
					break;
				int t = weights[xrand()%count];
				set(t, Increment);
			}
		}
		break;
	case Feats:
		if(v==Generate)
		{
			while(get(Feats)<get(FeatsAllowed))
			{
				// Для пользователей силы дадим обучение силе как можно быстрее
				if(allow(ForceTraining, 1))
				{
					if(!get(ForceTraining) || d100()<20)
					{
						set(ForceTraining, Increment);
						continue;
					}
				}
				int weights[LightSabers-AdeptNegotiator+1];
				int	count = get(weights, Feats, false);
				if(!count)
					break;
				int t = weights[xrand()%count];
				set(t, Increment);
			}
		}
		break;
	case Languages:
		if(v==Generate)
		{
			while(get(LanguagesPoints)>0)
			{
				int weights[ZabrakLanguage-Basic+1];
				int	count = get(weights, Languages, false);
				if(!count)
					break;
				int t = weights[xrand()%count];
				set(t, Increment);
			}
		}
		break;
	case ForcePowers:
		if(v==Generate)
		{
			while(get(ForcePowersPoints))
			{
				int	count = 0;
				int weights[VitalTransfer-BattleStrike+1];
				if(gattrc(BattleStrike, VitalTransfer, true)<setting::powers)
					count = get(weights, ForcePowers, false);
				else
					count = get(weights, ForcePowers, true);
				if(!count)
					break;
				int t = weights[xrand()%count];
				set(t, Increment);
			}
		}
		break;
	case Clear:
		if(v)
		{
			rmset(name, 0, sizeof(name));
			rmset(attr, 0, sizeof(attr));
			rmset(&wears, 0, sizeof(wears));
			rmset(posses, 0, sizeof(posses));
			attr[Str] = 10;
			attr[Dex] = 10;
			attr[Con] = 10;
			attr[Int] = 10;
			attr[Wis] = 10;
			attr[Cha] = 10;
		}
		break;
	case Equipment:
		if(v==Generate)
		{
			int r;
			wears.melee.clear();
			wears.second.clear();
			wears.ranged.clear();
			wears.armour.clear();
			if(get(HeavyWeapons))
			{
				r = d100();
				if(r<10)
					wears.ranged.create(BlasterHeavyRepeating);
				else if(r<30)
					wears.ranged.create(BlasterCannon);
				else if(r<60)
					wears.ranged.create(MissileLauncher);
				else
					wears.ranged.create(BlasterRifleHeavy);
			}
			switch(attr[Class])
			{
			case Jedi:
				wears.melee.create(LightSaber);
				break;
			case Noble:
				wears.ranged.create(BlasterPistol);
				break;
			case Scoundrel:
				if(!wears.ranged.isvalid())
					wears.ranged.create(BlasterPistol);
				wears.second.create(Knife);
				if(d100()<30)
					wears.armour.create(FlightSuitPadded);
				else
					wears.armour.create(BlastHelmetAndVest);
				break;
			case Scout:
				if(!wears.ranged.isvalid())
					wears.ranged.create(BlasterRifle);
				r = d100();
				if(r<10)
					wears.armour.create(FlightSuitPadded);
				else if(r<30)
					wears.armour.create(BlastHelmetAndVest);
				else if(r<60)
					wears.armour.create(FlightSuitArmored);
				else
					wears.armour.create(CombatJumpSuit);
				break;
			case Soldier:
				if(!wears.ranged.isvalid())
					wears.ranged.create(BlasterRifle);
				wears.second.create(Knife);
				r = d100();
				if(r<10)
					wears.armour.create(BattleArmourHeavy);
				else if(r<30)
					wears.armour.create(CombatJumpSuit);
				else if(r<60)
					wears.armour.create(FlightSuitArmored);
				else
					wears.armour.create(BattleArmour);
				p = (item*)gitem(EmpthySlot);
				if(p)
					p->create(GrenadeFrag,3);
				break;
			}
		}
		break;
	default:
		if(i<0)
			return false;
		if(i>=Jedi && i<=SithLord)
		{
			if(get(HeroicLevel)==0)
				attr[Class] = i;
			attr[i] = v;
		}
		else if(i<DarkSideScore)
		{
			if(v==Increment)
				attr[i]++;
			else if(v==Decrement)
			{
				if(attr[i]==0)
					return false;
				attr[i]--;
			}
			else
				attr[i] = v;
			return true;
		}
		else if(i>=Basic && i<=ZabrakLanguage)
		{
			if(v==Increment)
				attr[Languages] |= 1<<(i-Basic);
			else if(v==Decrement)
				attr[Languages] &= ~(1<<(i-Basic));
			return true;
		}
		return false;
	}
	return true;
}

bool unit::set(int t, const wchar_t* v)
{
	switch(t)
	{
	case Name:
		if(szcmp(v, L"random")==0)
		{
			bool woman = get(Gender)==Female;
			// Name
			if(d100()<40 || woman)
			{
				if(woman)
					szcat(name, human_female[xrand()%(sizeof(human_female)/sizeof(human_female[0]))]);
				else
					szcat(name, human_male[xrand()%(sizeof(human_male)/sizeof(human_male[0]))]);
			}
			else
			{
				szcat(name, human_male_prefics[xrand()%(sizeof(human_male_prefics)/sizeof(human_male_prefics[0]))]);
				szcat(name, human_male_postfics[xrand()%(sizeof(human_male_postfics)/sizeof(human_male_postfics[0]))]);
			}
			name[0] = chupper(name[0]);
			// Space
			szcat(name, L" ");
			// Family
			wchar_t* family = szend(name);
			szcat(name, human_family_begin[xrand()%(sizeof(human_family_begin)/sizeof(human_family_begin[0]))]);
			szcat(name, human_family_end[xrand()%(sizeof(human_family_end)/sizeof(human_family_end[0]))]);
			family[0] = chupper(family[0]);
		}
		else
			szcpy(name, v);
		break;
	default:
		return false;
	}
	return true;
}

int	unit::gattrcmax(const tokens t1, const tokens t2, int MaxLevel) const
{
	int c = 0;
	for(int i = t1; i<=t2; i++)
	{
		int j = get(i);
		int k = j - attr[i];
		c += (imin(j, MaxLevel)-k);
	}
	return c;
}

int	unit::gattrc(const tokens t1, const tokens t2, bool presence, int Level) const
{
	int c = 0;
	if(presence)
	{
		for(int i = t1; i<=t2; i++)
			if(attr[i]>=Level)
				c++;
	}
	else if(Level>=2)
	{
		for(int i = t1; i<=t2; i++)
		{
			int j = get(i);
			if(j<Level)
				continue;
			c += j-Level;
		}
	}
	else
	{
		for(int i = t1; i<=t2; i++)
			c += attr[i];
	}
	return c;
}

int	unit::gattrcbs(const tokens t1, const tokens t2) const
{
	int c = 0;
	for(int i = t1; i<=t2; i++)
	{
		if(!attr[i])
			continue;
		// Если не классовое умение, и оно стоит в 1 и более, значит это была
		// использована особенность и необходимо это учесть
		c += attr[i] - (allow((tokens)i, 1) ? 1 : 0);
	}
	return c;
}

bool unit::allow(int i, int m) const
{
	int k;
	switch(i)
	{
	case AcePilot:
		k = get(HeroicLevel);
		return k>=7 && attr[Pilot] && get(VehicularCombat)>0;
	case BountyHunter:
		return get(HeroicLevel)>=7 && attr[Survival] && gattrc(AcuteSeaser, UncannyDodgeII, false, 1)>0;
	case CrimeLord:
		return get(HeroicLevel)>=7
			&& attr[Deception]
			&& attr[Persuasion]
			&& gattrc(FoolsLuck, WalkTheLine, true, 1)>0;
	case EliteTrooper:
		return get(BaB)>=7
			&& get(ArmourProficience)>=2
			&& attr[MartialArts]>0
			&& get(PointBlankShoot)>0;
		//&& gattrc(ArmourMastery,ThoughtAsNail,true,1)>=1;
	case ForceAdept:
		return get(HeroicLevel)>=7
			&& attr[UseForce];
		//&& gattrc(DisciplinedStrike,Visions,true,1)>=3;
	case ForceDisciple:
		return get(HeroicLevel)>=12
			&& attr[UseForce]
			&& get(ForceSensitivity)
			&& attr[Farseeing];
		//&& gattrc(DisciplinedStrike,Visions,true,1)>=2;
	case GunSlinger:
		return get(HeroicLevel)>=7
			&& get(PreciseShoot)
			&& get(QuickDraw)
			&& get(Pistols);
	case JediKnight:
		return get(BaB)>=7
			&& attr[UseForce]
			&& get(ForceSensitivity)
			&& get(LightSaber)>=2;
	case JediMaster:
		return get(BaB)>=12
			&& attr[UseForce]
			&& get(ForceSensitivity)
			&& get(LightSaber)>=2;
	case Officer:
		return get(HeroicLevel)>=7
			&& attr[Tactics];
		//&& gattrc(BolsterAlly,InspireZeal,true,1)>0;
	case SithAppertice:
		return get(BaB)>=7
			&& attr[UseForce]
			&& get(ForceSensitivity)
			&& get(LightSaber)
			&& attr[DarkSideScore]>=attr[Wis];
	case SithLord:
		return get(BaB)>=12
			&& attr[UseForce]
			&& get(ForceSensitivity)
			&& get(LightSaber)
			&& attr[DarkSideScore]>=attr[Wis];
	case HeavyWeapons:
	case Rifles:
	case Pistols:
	case SimpleWeapons:
	case AdvancedMeleeWeapons:
		switch(m)
		{
		case 1:
		case 2:
			return true;
		case 3:
			return attr[Soldier]!=0;
		default:
			return false;
		}
	case LightSabers:
		switch(m)
		{
		case 1:
		case 2:
			return true;
		case 3:
			return attr[Jedi]!=0;
		default:
			return false;
		}
		// Навыки
	case Acrobatic:
		switch(m)
		{
		case 1:
			return attr[Jedi] || attr[Scoundrel];
		case 2:
			return true;
		default:
			return false;
		}
	case Climb:
		switch(m)
		{
		case 1:
			return attr[Scout] || attr[Soldier];
		case 2:
			return true;
		default:
			return false;
		}
	case Deception:
		switch(m)
		{
		case 1:
			return attr[Noble] || attr[Scoundrel];
		case 2:
			return true;
		default:
			return false;
		}
	case Endurance:
		switch(m)
		{
		case 1:
			return attr[Scout] || attr[Soldier] || attr[Jedi];
		case 2:
			return true;
		default:
			return false;
		}
	case GatherInformation:
		switch(m)
		{
		case 1:
			return attr[Noble] || attr[Scoundrel];
		case 2:
			return true;
		default:
			return false;
		}
	case Initiative:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return true;
		default:
			return false;
		}
	case Jump:
		switch(m)
		{
		case 1:
			return attr[Jedi] || attr[Scout] || attr[Soldier];
		case 2:
			return true;
		default:
			return false;
		}
	case Buerocracy:
	case GalacticLore:
	case LifeSciences:
	case PhysicalSciences:
	case SocialSciences:
	case Technology:
		switch(m)
		{
		case 1:
			return attr[Jedi] || attr[Scoundrel] || attr[Scout] || attr[Noble];
		case 2:
			return true;
		default:
			return false;
		}
	case Tactics:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return true;
		default:
			return false;
		}
	case Mechanics:
		switch(m)
		{
		case 1:
			return attr[Jedi] || attr[Scout] || attr[Soldier] || attr[Scoundrel];
		case 2:
			return true;
		default:
			return false;
		}
	case Perception:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return true;
		default:
			return false;
		}
	case Persuasion:
		switch(m)
		{
		case 1:
			return attr[Scoundrel] || attr[Noble];
		case 2:
			return true;
		default:
			return false;
		}
	case Pilot:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return true;
		default:
			return false;
		}
	case Ride:
		switch(m)
		{
		case 1:
			return attr[Scout] || attr[Noble];
		case 2:
			return true;
		default:
			return false;
		}
	case Stealth:
		switch(m)
		{
		case 1:
			return attr[Scoundrel] || attr[Scout];
		case 2:
			return true;
		default:
			return false;
		}
	case Survival:
		switch(m)
		{
		case 1:
			return attr[Scout]!=0;
		case 2:
			return true;
		default:
			return false;
		}
	case Swim:
		switch(m)
		{
		case 1:
			return attr[Scout] || attr[Soldier];
		case 2:
			return true;
		default:
			return false;
		}
	case TreatInjury:
		switch(m)
		{
		case 1:
			return attr[Noble] || attr[Soldier];
		case 2:
			return true;
		default:
			return false;
		}
	case UseComputer:
		switch(m)
		{
		case 1:
			return attr[Noble] || attr[Scoundrel] || attr[Soldier];
		case 2:
			return true;
		default:
			return false;
		}
	case UseForce:
		switch(m)
		{
		case 1:
			return attr[Jedi] || get(ForceSensitivity);
		case 2:
			return true;
		default:
			return false;
		}
		// feats
	case ArmourProficience:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return true;
		case 3:
			return true;
		default:
			return false;
		}
	case AcrobaticStrike:
		switch(m)
		{
		case 1:
			return attr[Acrobatic]!=0;
		default:
			return false;
		}
	case BanthaRush:
		switch(m)
		{
		case 1:
			return attr[Str]>=13 && get(BaB)>=1;
		default:
			return false;
		}
	case BurstFire:
		switch(m)
		{
		case 1:
			return attr[Str]>=13 && (get(HeavyWeapons)
				|| get(Pistols)
				|| get(Rifles));
		default:
			return false;
		}
	case CarefulShoot:
		switch(m)
		{
		case 1:
			return (get(BaB)>=2) && get(PointBlankShoot);
		default:
			return false;
		}
	case ChargingFire:
		switch(m)
		{
		case 1:
			return (get(BaB)>=4);
		default:
			return false;
		}
	case Cleave:
		switch(m)
		{
		case 1:
			return attr[Str]>=13 && get(PowerAttack);
		case 2:
			return attr[Str]>=13 && get(BaB)>=4;
		default:
			return false;
		}
	case CoordinatedAttack:
		switch(m)
		{
		case 1:
			return (get(BaB)>=2);
		default:
			return false;
		}
	case Crush:
		switch(m)
		{
		case 1:
			return (get(BaB)>=1) && get(Pin);
		default:
			return false;
		}
	case CyberneticSurgery:
		switch(m)
		{
		case 1:
			return attr[TreatInjury]!=0;
		default:
			return false;
		}
	case Deadeye:
		switch(m)
		{
		case 1:
			return get(PointBlankShoot) && get(PreciseShoot) && get(BaB)>=4;
		default:
			return false;
		}
	case Dodge:
		switch(m)
		{
		case 1:
			return attr[Dex]>=13;
		default:
			return false;
		}
	case DoubleAttack:
		switch(m)
		{
		case 1:
			return get(BaB)>=6;
		default:
			return false;
		}
	case DreadfulRage:
		switch(m)
		{
		case 1:
			return get(BaB)>=1 && get(Rage);
		default:
			return false;
		}
	case DualWeaponMastery:
		switch(m)
		{
		case 1:
			return attr[Dex]>=13 && get(BaB)>=1;
		case 2:
			return attr[Dex]>=15 && get(BaB)>=6;
		case 3:
			return attr[Dex]>=17 && get(BaB)>=11;
		default:
			return false;
		}
	case ExoticWeapon:
		switch(m)
		{
		case 1:
			return get(BaB)>=1;
		default:
			return false;
		}
	case ExtraRage:
		switch(m)
		{
		case 1:
			return true;
		default:
			return false;
		}
	case ExtraSecondWind:
		switch(m)
		{
		case 1:
			return attr[Endurance]!=0;
		default:
			return false;
		}
	case FarShoot:
		switch(m)
		{
		case 1:
			return get(PointBlankShoot)!=0;
		default:
			return false;
		}
	case ForceBoon:
		return get(ForceSensitivity)!=0;
	case ForceTraining:
		return get(ForceSensitivity)!=0;
	case ImprovedCharge:
		switch(m)
		{
		case 1:
			return get(Mobility)!=0;
		default:
			return false;
		}
	case ImprovedDisarm:
		switch(m)
		{
		case 1:
			return get(MeleeDefence)!=0;
		default:
			return false;
		}
	case Linguist:
		switch(m)
		{
		case 1:
			return attr[Int]>=13;
		default:
			return false;
		}
	case MartialArts:
		switch(m)
		{
		case 1:
			return true;
		case 2:
			return get(BaB)>=3;
		case 3:
			return get(BaB)>=6;
		default:
			return false;
		}
	case MightySwing:
		switch(m)
		{
		case 1:
			return attr[Str]>=13;
		default:
			return false;
		}
	case Mobility:
		switch(m)
		{
		case 1:
			return attr[Dex]>=13 && get(Dodge);
		default:
			return false;
		}
	case Pin:
		switch(m)
		{
		case 1:
			return get(BaB)>=1;
		default:
			return false;
		}
	case PowerAttack:
		switch(m)
		{
		case 1:
			return attr[Str]>=13;
		default:
			return false;
		}
	case PowerfulCharge:
		switch(m)
		{
		case 1:
			return attr[Str]>=13;
		default:
			return false;
		}
	case PreciseShoot:
		switch(m)
		{
		case 1:
			return get(PointBlankShoot)!=0;
		default:
			return false;
		}
	case QuickDraw:
		switch(m)
		{
		case 1:
			return get(BaB)>=1;
		default:
			return false;
		}
	case RapidShoot:
		switch(m)
		{
		case 1:
			return attr[Str]>=13 && get(BaB)>=1;
		default:
			return false;
		}
	case RapidStrike:
		switch(m)
		{
		case 1:
			return attr[Dex]>=13 && get(BaB)>=1;
		default:
			return false;
		}
	case ShakeItOff:
		switch(m)
		{
		case 1:
			return attr[Con]>=13 && attr[Endurance];
		default:
			return false;
		}
	case Sniper:
		switch(m)
		{
		case 1:
			return get(PreciseShoot) && get(BaB)>=4;
		default:
			return false;
		}
	case SurgicalExpertise:
		switch(m)
		{
		case 1:
			return attr[TreatInjury]!=0;
		default:
			return false;
		}
	case Throw:
		switch(m)
		{
		case 1:
			return get(Trip) && get(BaB)>0;
		default:
			return false;
		}
	case Trip:
		switch(m)
		{
		case 1:
			return get(BaB)>0;
		default:
			return false;
		}
	case TripleAttack:
		switch(m)
		{
		case 1:
			return get(DoubleAttack) && get(BaB)>=9;
		default:
			return false;
		}
	case TripleCrit:
		switch(m)
		{
		case 1:
			return get(BaB)>=8;
		default:
			return false;
		}
	case VehicularCombat:
		switch(m)
		{
		case 1:
			return attr[Pilot]!=0;
		default:
			return false;
		}
	case WeaponFiness:
		switch(m)
		{
		case 1:
			return get(BaB)>=1;
		default:
			return false;
		}
	case WhirlwindAttack:
		switch(m)
		{
		case 1:
			return attr[Dex]>=13 && attr[Int]>=13 && get(MeleeDefence) && get(BaB)>=4;
		default:
			return false;
		}
		// Force powers
	case BattleStrike:
	case DarkRage:
	case Farseeing:
	case ForceDisarm:
	case ForceGrip:
	case ForceLighting:
	case ForceSlam:
	case ForceStun:
	case ForceThrust:
	case MindTrick:
	case MoveObject:
	case NegateEnergy:
	case Rebuke:
	case Surge:
		return get(ForceSensitivity)!=0;
	case SeverForce:
	case VitalTransfer:
		return attr[DarkSideScore]==0 && get(ForceSensitivity);
		// Jedi
	case ForcePersuasion:
	case DarkSideScourge:
		return get(DarkSideSence)!=0;
	case ForceHaze:
		return get(ClearMind)!=0;
	case ResistDarkSide:
		return get(DarkSideSence)!=0;
	case RedirectShot:
		return get(Deflect) && get(BaB)>=5;
	case AdeptNegotiator:
		switch(m)
		{
		case 1:
			return attr[Jedi]!=0;
		case 2:
			return attr[Jedi]!=0;
		default:
			return false;
		}
	case LightsaberDefence:
		switch(m)
		{
		case 1:
		case 2:
		case 3:
			return attr[Jedi]!=0;
		default:
			return false;
		}
	case AcrobaticRecovery:
	case SkilledAdvisor:
	case BattleMeditation:
	case ElusiveTarget:
	case ForceIntuition:
	case Resilience:
	case ClearMind:
	case DarkSideSence:
	case Block:
	case Deflect:
	case LightSaberThrow:
		switch(m)
		{
		case 1:
			return attr[Jedi]!=0;
		default:
			return false;
		}
		// Noble
	case DemandSurrender:
	case WeakenResolve:
		return get(Presence)!=0;
	case ImprovedWeakenResolve:
		return get(WeakenResolve)!=0;
	case IgniteFerver:
		return get(InspireConfidence)!=0;
	case InspireZeal:
		return get(IgniteFerver)!=0;
	case DistantCommand:
	case FearlessLeader:
		return get(BornLeader)!=0;
	case Rally:
		return get(DistantCommand)!=0;
	case Trust:
		return get(BornLeader) && get(Coordinate);
	case SpontaneousSkill:
		return get(Educated)!=0;
	case BornLeader:
	case BolsterAlly:
	case Coordinate:
	case InspireConfidence:
	case InspireHaste:
	case Presence:
	case Connections:
	case Educated:
	case Wealth:
		switch(m)
		{
		case 1:
			return attr[Noble]!=0;
		default:
			return false;
		}
		// Scoundrel
	case LuckyShoot:
		return get(Knack)!=0;
	case WalkTheLine:
		return get(Disruptive)!=0;
	case MasterSlicer:
		return get(Gimmick)!=0;
	case StarshipRaider:
	case StellarWarrior:
		return get(Spacehound)!=0;
	case Gambler:
	case Knack:
		switch(m)
		{
		case 1:
			return attr[Scoundrel]!=0;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			return true;
		default:
			return false;
		}
	case FoolsLuck:
	case FortuneFavor:
	case BastardlyStrike:
	case Disruptive:
	case Skirmisher:
	case SneakAttack:
	case Gimmick:
	case Tracer:
	case Hyperdriven:
	case Spacehound:
		switch(m)
		{
		case 1:
			return attr[Scoundrel]!=0;
		default:
			return false;
		}
	case ExpertTracker:
	case ImprovedInitiative:
	case KeenShot:
		return get(AcuteSeaser)!=0;
	case UncannyDodgeI:
		return get(ImprovedInitiative)!=0;
	case UncannyDodgeII:
		return get(UncannyDodgeI)!=0;
	case HiddenMovement:
		return get(ImprovedStealth)!=0;
	case TotalConcealment:
		return get(HiddenMovement)!=0;
	case AcuteSeaser:
	case ImprovedStealth:
	case Barter:
	case FringeSavant:
	case LongStride:
	case JuryRigger:
	case Evasion:
	case ExtremeEffort:
	case Sprint:
	case Surefooted:
		switch(m)
		{
		case 1:
			return attr[Scout]!=0;
		default:
			return false;
		}
		// Soldier
	case ArmourMastery:
	case ImprovedArmoredDefence:
	case Juggernaut:
	case SecondSkin:
		return get(ArmoredDefence)!=0;
	case StunningStrike:
		return get(MeleeSmash)!=0;
	case UnbalanceOpponent:
		return get(ExpertGrappler)!=0;
	case CoverFire:
		return get(BattleAnalysis)!=0;
	case HarmsWay:
		return get(Initiative) && attr[Soldier];
	case ArmoredDefence:
	case ExpertGrappler:
	case GunClub:
	case MeleeSmash:
	case BattleAnalysis:
	case DrawFire:
	case Indomitable:
	case DevastatingAttack:
	case PenetratingAttack:
	case ThoughtAsNail:
		switch(m)
		{
		case 1:
			return attr[Soldier]!=0;
		default:
			return false;
		}
	case Demolitionist:
		switch(m)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			return attr[Soldier]!=0;
		default:
			return false;
		}
	case ForceRecovery:
		return get(Equilibrium)!=0;
	case TelekineticPower:
	case TelekineticSavant:
		return get(ForceSensitivity) && (attr[ForceDisarm] || attr[ForceGrip] || attr[ForceSlam] || attr[ForceThrust] || attr[MoveObject]);
	case DarkPresence:
		return attr[Cha]>=13 && get(PowerOfDarkSide);
	case Revenge:
		return get(DarkPresence)!=0;
	case SwiftPower:
		return get(PowerOfDarkSide)!=0;
	case DisciplinedStrike:
	case DamageReduction10:
	case Equilibrium:
	case ForceFocus:
	case PowerOfDarkSide:
	case ForcePerception:
	case ForcePilot:
	case Foresight:
	case GaugeForcePotential:
	case Visions:
		return get(ForceSensitivity)!=0;
	case ElusiveDogFighter:
	case FullThrottle:
	case VehicularEvasion:
	case KeepItTogether:
	case RelentlessPursuit:
	case ExpertGunner:
		switch(m)
		{
		case 1:
			return attr[AcePilot]!=0;
		default:
			return false;
		}
	case Juke:
		switch(m)
		{
		case 1:
			return attr[VehicularEvasion]!=0;
		default:
			return false;
		}
	case DogfightGunner:
	case QuickTrigger:
	case SystemHit:
		switch(m)
		{
		case 1:
			return attr[ExpertGunner]!=0;
		default:
			return false;
		}
		// Охотник за наживой
	case HuntersMark:
	case Notorious:
	case NowhereToHide:
		switch(m)
		{
		case 1:
			return attr[BountyHunter]!=0;
		default:
			return false;
		}
	case Relentless:
		switch(m)
		{
		case 1:
			return attr[HuntersMark] && attr[HuntersTarget];
		default:
			return false;
		}
	case HuntersTarget:
		switch(m)
		{
		case 1:
			return attr[HuntersMark]!=0;
		default:
			return false;
		}
	case RushlessNegotiator:
		switch(m)
		{
		case 1:
			return attr[Notorious]!=0;
		default:
			return false;
		}
	default:
		if(i<=DarkSideScore)
			return m<1;
		else if(i==Basic)
			return false;
		else if(i<=LastLanguage)
			return (attr[Languages]&(1<<(i-FirstLanguage)))==0;
		return false;
	}
}

static int get_level_by_exp(int e)
{
	int i = 1;
	for(; experience_table[i] && e>=experience_table[i]; i++);
	return i;
}

int unit::gclassc() const
{
	int c = 0;
	for(int i = Jedi; i<=Soldier; i++)
	{
		if(get((tokens)i))
			c++;
	}
	return c;
}

void unit::levelup(int type)
{
	// check minimum rolled hits
	int m = get(HeroicLevel)-1;
	if(m>0 && get(HitsRolled)<m)
		set(HitsRolled, m);
	// attribute raised each 4 level
	int n = get(HeroicLevel)+1;
	if((n%4)==0)
	{
		switch(get(Class))
		{
		case Jedi:
			attr[Wis]++;
			if(xrand()%2)
				attr[Dex]++;
			else
				attr[Str]++;
			break;
		case Noble:
			attr[Cha]++;
			if(xrand()%2)
				attr[Int]++;
			else
				attr[Wis]++;
			break;
		case Scoundrel:
			attr[Dex]++;
			if(xrand()%2)
				attr[Int]++;
			else
				attr[Cha]++;
			break;
		default:
			attr[Dex]++;
			if(xrand()%2)
				attr[Str]++;
			else
				attr[Con]++;
			break;
		}
	}
	set(Class, type);
	set(Skills, Generate);
	set(Talents, Generate);
	set(Feats, Generate);
	set(ForcePowers, Generate);
	set(Languages, Generate);
}

void unit::random(int race, int type, int gender, int exp)
{
	if(!race)
		race = Human + xrand()%(Wookie-Human+1);
	if(!type)
		type = Jedi + xrand()%(Soldier-Jedi+1);
	if(!gender)
		gender = Male + xrand()%2;
	set(Clear, 1);
	set(Abilities, Generate);
	set(Race, race);
	set(Gender, gender);
	set(Abilities, type);
	set(Exp, exp);
	// random level up
	levelup(type);
	int m = get_level_by_exp(get(Exp));
	while(get(HeroicLevel)<m)
		levelup(type);
	// install maximum hit points
	set(Hits, get(HitsTotal));
	set(Equipment,Generate);
	set(Name, L"random");
}

int unit::roll(int i, int d, int m, int& critical) const
{
	int b = get(i) + m;
	int r = (xrand()%20)+1;
	switch(i)
	{
	case Initiative:
		if(get(IntuitiveInitiative) && r<8)
			r = (xrand()%20)+1;
		if(get(ImprovedInitiative) && r<8)
			r = (xrand()%20)+1;
		break;
	case Pilot:
		if(Race==Duros && (r+b)<d)
			r = (xrand()%20)+1;
		break;
	case Stealth:
		if((r+b)<d && Race==Ewoks)
			r = (xrand()%20)+1;
		break;
	case Swim:
		if((r+b)<d && get(ExpertSwimmer))
			r = (xrand()%20)+1;
		break;
	case Survival:
		if((r+b)<d && get(SurvivalInstinct))
			r = (xrand()%20)+1;
		break;
	default:
		break;
	}
	return (r+b)-d;
}

bool unit::add(item it, bool run)
{
	for(unsigned i = 0; i<sizeof(posses)/sizeof(posses[0]); i++)
	{
		if(!posses[i].isvalid())
		{
			if(run)
				posses[i] = it;
			return true;
		}
		if(posses[i]==it && posses[i].add(it, false))
		{
			if(run)
				posses[i].add(it, true);
			return true;
		}
	}
	return false;
}

bool unit::del(item it, bool run)
{
	for(unsigned i = 0; i<sizeof(posses)/sizeof(posses[0]); i++)
	{
		if(posses[i]==it && posses[i].del(1, false))
		{
			if(run)
				posses[i].del(1, true);
			return true;
		}
	}
	return false;
}

void swap(item& i1, item& i2)
{
	item i = i2;
	i2 = i1;
	i1 = i;
}

bool unit::equip(item* it, bool run)
{
	if(!it)
		return false;
	int i = it->get(BaseClass);
	int m = 1;
	switch(i)
	{
	case ArmourHeavy:
		m++;
	case ArmourMedium:
		m++;
	case ArmourLight:
		if(!allow(ArmourProficience, m))
			return false;
		if(run)
			swap(*it, wears.armour);
		break;
	case AdvancedMeleeWeapons:
	case LightSabers:
	case SimpleWeapons:
		if(!allow(i, 1))
			return false;
		if(run)
			swap(*it, wears.melee);
		break;
	case HeavyWeapons:
	case Rifles:
	case Pistols:
		if(!allow(i, 1))
			return false;
		if(run)
			swap(*it, wears.ranged);
		break;
	default:
		return false;
	}
	return true;
}

int unit::gbonusref(bool FlatFooted) const
{
	int r = gbonus(Dex);
	if(r>0 && FlatFooted)
		r = 0;
	if(wears.armour.isvalid())
	{
		int md = wears.armour.get(Dex);
		if(r>md)
			r = md;
		r += wears.armour.get(Reflexes);
	}
	else
		r += get(HeroicLevel);
	return r;
}

int unit::gbonusfort() const
{
	if(wears.armour.isvalid())
		return wears.armour.get(Fortitude);
	else
		return 0;
}

void unit::get(int i, wchar_t* text, int m, int n) const
{
	static const wchar_t* SkillsRangName[][2][LocaleCount] =
	{
		{{L"не владеет", L"non-profecient"}, {L"-5 к атаке с этим типом оружия.", L"-5 to attack with this type of weapon."}},
		{{L"владеет", L"profecient"}, {L"Вы используете этот тип оружия без штрафов и ограничений.", L"You can use this type of weapon without any penalty."}},
		{{L"эксперт по", L"expert"}, {L"+1 к атаке с этим типом оружия и знаете большинство самых распространенных моделей.", L"+1 to attack with this type of weapon. You known all common models of this weapon group."}},
		{{L"мастер по", L"master"}, {L"+1 к атаке и +2 к урону с этим типом оружия и знаете все распространенные модели.", L"+1 to attack and +2 to damage with this type of weapon. You known all common models of this weapon group."}},
		{{L"великий мастер по", L"great master"}, {L"+2 к атаке и +2 к урону с этим типом оружия.", L"+2 to attack and +2 to damage with this type of weapon."}},
		{{L"сенсей по", L"superb master"}, {L"+2 к атаке и +4 к урону с этим типом оружия.", L"+2 to attack and +4 to damage with this type of weapon."}}
	};
	static const wchar_t* ArmourProficienceDecription[][2][2] =
	{
		{{L"не использует броню", L"no use armour"}, {L"", L""}},
		{{L"носит легкую броню", L"wear light armour"}, {L"Носит легкую броню без штрафов и использует ее бонусы оборудования.", L"Wear light armour without penalty and use it's equipment bonus."}},
		{{L"носит среднюю броню", L"wear medium armour"}, {L"Носит легкую и среднюю броню без штрафов и использует ее бонусы оборудования.", L"Wear medium armour without penalty and use it's equipment bonus."}},
		{{L"носит тяжелую броню", L"wear heavy armour"}, {L"Носит любую броню без штрафов и использует ее бонусы оборудования.", L"Wear any armour without penalty and use it's equipment bonus."}},
	};
	static const wchar_t* SkillsDescription[UseForce-Acrobatic+1][LocaleCount] =
	{
		{L"акробатике", L"acobatic"},
		{L"лазанию", L"climb"},
		{L"обману", L"bluff"},
		{L"выносливости", L"endurance"},
		{L"добыванию информации", L"gather information"},
		{L"инициативе", L"initiative"},
		{L"прыжкам", L"jump"},
		{L"бюрократии", L"beurocraty"},
		{L"галактическим знаниям", L"galactic lore"},
		{L"биологическим знаниям", L"biology"},
		{L"физическим наукам", L"physics"},
		{L"социальным знаниям", L"social"},
		{L"тактике", L"tactic"},
		{L"технологиям", L"technology"},
		{L"механике", L"mechanics"},
		{L"восприятию", L"perception"},
		{L"убеждению", L"persuade"},
		{L"пилотированию", L"pilot"},
		{L"езде верхом", L"ride"},
		{L"скрытности", L"stealth"},
		{L"выживанию", L"survival"},
		{L"плаванию", L"swim"},
		{L"обработке ран", L"treat wound"},
		{L"использованию компьютера", L"use computer"},
		{L"использованию силы",L"use force"},
	};
	static const wchar_t* SkillRangDescription[] =
	{
		L"Навык используется без бонусов.",
		L"Вы обучены навыку и получаете +5 бонус от обучения.",
		L"+2 бонус от обучения дополнительно (итого бонус +7) и более глубокие знания.",
		L"+2 бонус от обучения дополнительно (итого бонус +9) и уникальные знания.",
	};
	static const wchar_t* WeaponGroupsDescription[LightSabers-SimpleWeapons+1][LocaleCount] =
	{
		{L"простому оружию", L"simple weapon"},
		{L"улучшенному оружию ближнего боя", L"improved melee weapon"},
		{L"пистолетам", L"pistols"},
		{L"винтовкам", L"rifles"},
		{L"тяжелому вооружению", L"heavy weapons"},
		{L"световому мечу", L"light saber"},
	};
	static const wchar_t* WeaponGroupsDescription2[LightSabers-SimpleWeapons+1][LocaleCount] =
	{
		{L"простым оружием", L"simple weapons"},
		{L"улучшенным оружием ближнего боя", L"improved melee weapons"},
		{L"пистолетами", L"pistols"},
		{L"винтовками", L"rifles"},
		{L"тяжелым вооружением", L"heavy weapons"},
		{L"световым мечем", L"light sabers"},
	};
	switch(i)
	{
	case Race:
		return get(get(Race), text, m, n);
	case ArmourProficience:
		if(n==0)
			szcat(text, ArmourProficienceDecription[m][0][locale]);
		else
			szcat(text, ArmourProficienceDecription[m][1][locale]);
		break;
	case Acrobatic:
	case Climb:
	case Deception:
	case Endurance:
	case GatherInformation:
	case Initiative:
	case Jump:
	case Buerocracy:
	case GalacticLore:
	case LifeSciences:
	case PhysicalSciences:
	case SocialSciences:
	case Tactics:
	case Technology:
	case Mechanics:
	case Perception:
	case Persuasion:
	case Pilot:
	case Ride:
	case Stealth:
	case Survival:
	case Swim:
	case TreatInjury:
	case UseComputer:
	case UseForce:
		if(n==0)
		{
			szcat(text, SkillsRangName[m][0][locale]);
			szcat(text, L" ");
			szcat(text, SkillsDescription[i-Acrobatic][locale]);
		}
		else
			szcat(text, SkillRangDescription[m]);
		break;
	case SimpleWeapons:
	case AdvancedMeleeWeapons:
	case Pistols:
	case Rifles:
	case HeavyWeapons:
	case LightSabers:
		if(n==0)
		{
			szcat(text, SkillsRangName[m][0][locale]);
			szcat(text, L" ");
			if(m>1)
				szcat(text, WeaponGroupsDescription[i-SimpleWeapons][locale]);
			else
				szcat(text, WeaponGroupsDescription2[i-SimpleWeapons][locale]);
		}
		else
			szcat(text, SkillsRangName[m][1][locale]);
		break;
	default:
		if(n==0)
		{
			static const wchar_t* StringRim[] = {L"", L"I", L"II", L"III", L"IV", L"V", L"VI"};
			switch(i)
			{
			case Cleave:
			case DualWeaponMastery:
			case MartialArts:
				szprint(szend(text), L"%1 %2", gstr(i), StringRim[m]);
				break;
			default:
				szcat(text, gstr(i));
				if(m>1)
					szprint(text+szlen(text), L" x%1i", m);
				break;
			}
		}
		else
		{
			damage dice;
			switch(i)
			{
			case CombatReflexes:
				szprint(text+szlen(text), gdescription(i), imax(2, 1+gbonus(Dex)));
				break;
			case Toughness:
			case ImprovedDefences:
			case AdeptNegotiator:
			case LightsaberDefence:
			case SkillTraining:
			case SneakAttack:
			case Knack:
			case NaturalArmour:
				szprint(text+szlen(text), gdescription(i), m);
				break;
			case IronWill:
			case GreatFortitude:
			case LightingReflexes:
			case Gambler:
				szprint(text+szlen(text), gdescription(i), m*2);
				break;
			case Linguist:
				szprint(text+szlen(text), gdescription(i), get(LanguagesAllowed));
				break;
			case DualWeaponMastery:
				szprint(text+szlen(text), gdescription(i), DualWeaponMasteryPenalty[m]);
				break;
			case ForceTraining:
				szprint(text+szlen(text), gdescription(i), get(ForcePowersAllowed));
				break;
			case DarkSideScourge:
				szprint(text+szlen(text), gdescription(i), imax(1, gbonus(Cha)));
				break;
			case DisciplinedStrike:
				szprint(text+szlen(text), gdescription(i), imax(1, gbonus(Wis)));
				break;
			case ForceHaze:
				szprint(text+szlen(text), gdescription(i), get(Jedi));
				break;
			case MartialArts:
				gdamage(item(), dice, m-get(MartialArts));
				szprint(text+szlen(text), gdescription(i), dice.tostring(), m);
				break;
			default:
				if(i>=FirstLanguage && i<=LastLanguage)
					szprint(szend(text), szt(L"Вы понимаете %1 язык.", L"You undestand %1 language."), gstr(i));
				else
					szcpy(text+szlen(text), gdescription(i));
				break;
			}
		}
		break;
	}
}

void unit::gdamage(const item it, damage& result, int boost) const
{
	static damage unarmed[] = {{1, 1}, {1, 3}, {1, 4}, {1, 6}, {1, 8}, {1, 10}};
	if(it.isvalid())
	{
		result.bonus = get(BaB);
		if(it.get(PrimaryAbility)==Missile)
			result.bonus += gbonus(Dex);
		else if(get(WeaponFiness)>0 && it.get(Size)<get(Size))
		{
			result.bonus += gbonus(Dex);
			result.modifier += gbonus(Str);
		}
		else
		{
			result.bonus += gbonus(Str);
			result.modifier += gbonus(Str);
			if(it.get(Size)>=get(Size) && !wears.second.isvalid())
				result.modifier += gbonus(Str);
		}
		it.gdamage(result);
		int m = get(it.get(BaseClass));
		result.bonus += SkillsAttackBonus[m];
		result.modifier += skills_damage_bonus[m];
	}
	else
	{
		result = unarmed[get(Size)-Tiny+get(MartialArts)+boost];
		if(get(WeaponFiness)>0 && (get(Dex)>get(Str)))
		{
			result.bonus += gbonus(Dex);
			result.modifier += gbonus(Str);
		}
		else
		{
			result.bonus += gbonus(Str);
			result.modifier += gbonus(Str);
		}
	}
}

void unit::get(item it, wchar_t* text) const
{
	damage d;
	switch(it.get(BaseClass))
	{
	case SimpleWeapons:
	case AdvancedMeleeWeapons:
	case Pistols:
	case Rifles:
	case HeavyWeapons:
	case LightSabers:
		if(text[0])
			szcat(text, L" ");
		szcat(text, L"[");
		d.clear();
		gdamage(it, d);
		szcat(text, d.tostring());
		szcat(text, L"]");
		break;
	default:
		break;
	}
}

int unit::get(int* data, int id, bool weared) const
{
	int maximum = 0;
	switch(id)
	{
	case Feats:
		if(weared)
		{
			// Feats
			for(int i = AcrobaticStrike; i<=WhirlwindAttack; i++)
				if(get(i))
					data[maximum++] = i;
			// Skills only for focus
			for(int i = FirstSkill; i<=LastSkill; i++)
				if(attr[i]>=2)
					data[maximum++] = i;
			// Weapon skills
			for(int i = SimpleWeapons; i<=LightSabers; i++)
			{
				int id = get(i);
				if(id==1 || id==2)
					data[maximum++] = i;
			}
			// Species traits
			for(int i = Primitive; i<=Scent; i++)
				if(get(i))
					data[maximum++] = i;
		}
		else if(get(FeatsPoints))
		{
			// Feats
			for(int i = AcrobaticStrike; i<=WhirlwindAttack; i++)
				if(allow(i, get(i)+1))
					data[maximum++] = i;
			// Skills only for focus
			for(int i = FirstSkill; i<=LastSkill; i++)
				if(attr[i]==1)
					data[maximum++] = i;
			// Weapon skills
			for(int i = SimpleWeapons; i<=LightSabers; i++)
			{
				int id = get(i);
				if(id==1 && allow(i, 2))
					data[maximum++] = i;
			}
		}
		break;
	case Talents:
		if(weared)
		{
			for(int i = AdeptNegotiator; i<=Visions; i++)
				if(get(i))
					data[maximum++] = i;
			for(int i = SimpleWeapons; i<=LightSabers; i++)
				if(get(i)==3)
					data[maximum++] = i;
		}
		else if(get(TalentsPoints))
		{
			for(int i = AdeptNegotiator; i<=Visions; i++)
				if(allow(i, get(i)+1))
					data[maximum++] = i;
			for(int i = SimpleWeapons; i<=LightSabers; i++)
			{
				int id = get(i);
				if(id>=2 && allow(i, id+1))
					data[maximum++] = i;
			}
		}
		break;
	case ForcePowers:
		if(weared)
		{
			for(int i = BattleStrike; i<=VitalTransfer; i++)
				if(get(i))
					data[maximum++] = i;
		}
		else if(get(ForcePowersPoints))
		{
			for(int i = BattleStrike; i<=VitalTransfer; i++)
				if(allow(i, get(i)+1))
					data[maximum++] = i;
		}
		break;
	case Languages:
		if(weared)
		{
			for(int i = FirstLanguage; i<=LastLanguage; i++)
				if(get(i))
					data[maximum++] = i;
		}
		else if(get(LanguagesPoints))
		{
			for(int i = FirstLanguage; i<=LastLanguage; i++)
				if(allow(i, get(i)+1))
					data[maximum++] = i;
		}
		break;
	}
	return maximum;
}

static const unit* compare_by_name_hero;

static int compare_by_name(const void* c1, const void* c2, void* param)
{
	wchar_t t1[260]; t1[0] = 0;
	wchar_t t2[260]; t2[0] = 0;
	int i1 = *((int*)c1);
	int i2 = *((int*)c2);
	if(i1>=FirstSkill && i1<=LastSkill)
		compare_by_name_hero->get(i1, t1, compare_by_name_hero->attr[i1], 0);
	else
		compare_by_name_hero->get(i1, t1, compare_by_name_hero->get(i1), 0);
	if(i2>=FirstSkill && i2<=LastSkill)
		compare_by_name_hero->get(i2, t2, compare_by_name_hero->attr[i2], 0);
	else
		compare_by_name_hero->get(i2, t2, compare_by_name_hero->get(i2), 0);
	t1[0] = chupper(t1[0]);
	t2[0] = chupper(t2[0]);
	return szcmp(t1, t2);
}

static int compare_by_name_try_to_add(const void* c1, const void* c2, void* param)
{
	wchar_t t1[128]; t1[0] = 0;
	wchar_t t2[128]; t2[0] = 0;
	int i1 = *((int*)c1);
	int i2 = *((int*)c2);
	if(i1>=FirstSkill && i1<=LastSkill)
		compare_by_name_hero->get(i1, t1, compare_by_name_hero->attr[i1]+1, 0);
	else
		compare_by_name_hero->get(i1, t1, compare_by_name_hero->get(i1)+1, 0);
	if(i2>=FirstSkill && i2<=LastSkill)
		compare_by_name_hero->get(i2, t2, compare_by_name_hero->attr[i2]+1, 0);
	else
		compare_by_name_hero->get(i2, t2, compare_by_name_hero->get(i2)+1, 0);
	t1[0] = chupper(t1[0]);
	t2[0] = chupper(t2[0]);
	return szcmp(t1, t2);
}

void unit::sort(int* data, unsigned count, bool weared) const
{
	compare_by_name_hero = this;
	if(weared)
		xsort(data, count, sizeof(int), compare_by_name, 0);
	else
		xsort(data, count, sizeof(int), compare_by_name_try_to_add, 0);
}

static bool isstr(int id)
{
	switch(id)
	{
	case Name:
		return true;
	default:
		return false;
	}
}

static bool isref(int id)
{
	switch(id)
	{
	case Class:
	case Race:
	case Gender:
		return true;
	default:
		return false;
	}
}

static bool isitem(int id)
{
	switch(id)
	{
	case Melee:
	case Missile:
	case Armour:
	case Slot1:
	case Slot2:
	case Slot3:
	case Slot4:
	case Slot5:
	case Slot6:
	case Slot7:
	case Slot8:
		return true;
	default:
		return false;
	}
}

//void write(xml::writer& doc, unit& e)
//{
//	const int idstr[] = {Name};
//	const int iditem[] = {Melee, Missile, Armour, Slot1, Slot2, Slot3, Slot4, Slot5, Slot6, Slot7, Slot8};
//	wchar_t temp[260];
//	doc.open(L"character");
//	for(auto i : idstr)
//	{
//		e.get(i, temp);
//		doc.open(L"field");
//		doc.attribute(L"id", gtag(i));
//		doc.attribute(L"value", temp);
//		doc.close(L"field");
//	}
//	for(auto i : iditem)
//	{
//		const item* p = e.gitem(i);
//		if(!p || !p->isvalid())
//			continue;
//		doc.open(L"field");
//		doc.attribute(L"id", gtag(i));
//		doc.attribute(L"value", gtag(p->get(Class)));
//		doc.attribute(L"count", p->get(Count));
//		doc.close(L"field");
//	}
//	for(int i = Jedi; i<=DarkSideScore; i++)
//	{
//		if(!e.attr[i])
//			continue;
//		doc.open(L"field");
//		doc.attribute(L"id", gtag(i));
//		if(isref(i))
//			doc.attribute(L"value", gtag(e.attr[i]));
//		else
//			doc.attribute(L"value", e.attr[i]);
//		doc.close(L"field");
//	}
//	doc.close(L"character");
//}

int gid(const wchar_t* tag)
{
	for(int i = 0; i<=ChooseGender; i++)
	{
		const wchar_t* p = gtag(i);
		if(szcmpi(p, tag)==0)
			return i;
	}
	return 0;
}

//void read(xml::reader& doc, unit& e)
//{
//	for(doc.next(); doc.type==xml::NameBegin && doc.element==L"character"; doc.next())
//	{
//		for(doc.next(); doc.type==xml::NameBegin && doc.element==L"field"; doc.next())
//		{
//			wchar_t temp[260];
//			int id = 0;
//			int value = 0;
//			int count = 1;
//			temp[0] = 0;
//			for(doc.next(); doc.type==xml::Attribute; doc.next())
//			{
//				if(doc.attribute==L"id")
//					id = gid(doc.attribute.value);
//				else if(doc.attribute==L"value")
//				{
//					if(isstr(id))
//						szcpy(temp, doc.attribute.value);
//					else if(isref(id))
//						value = gid(doc.attribute.value);
//					else if(isitem(id))
//						value = gid(doc.attribute.value);
//					else
//						value = sz2num(doc.attribute.value);
//				}
//				else if(doc.attribute==L"count")
//					count = sz2num(doc.attribute.value);
//			}
//			if(isstr(id))
//				e.set(id, temp);
//			else if(isitem(id))
//			{
//				item* p = (item*)e.gitem(id);
//				if(p)
//					p->create(value, count);
//			}
//			else
//				e.attr[id] = value;
//		}
//	}
//}