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

const int wkg = 10;

#define	DTSlash		1
#define	DTBludg		2
#define	DTPierc		4
#define	DTEnerg		8
#define	DTStun		16
#define	DTIon		32
#define	RoFA		64
#define	RoFS		128
#define	DTFire		256
#define	AttArea		256*2

struct itemd
{
	tokens			Group;
	tokens			size;
	int				Cost;
	damage			Damage;
	unsigned		flags;
	int				weight;
	tokens			available;
};

static itemd		data[] = {
	/*Empthy*/{None},
	/*Vibrodagger*/{AdvancedMeleeWeapons, Tiny, 200, {2, 4}, DTSlash|DTPierc, 10, AvaibleNormal},
	/*Vibroblade*/{AdvancedMeleeWeapons, Small, 250, {2, 6}, DTSlash|DTPierc, 18, Licensed},
	/*Vibrobayonet*/{AdvancedMeleeWeapons, Medium, 350, {2, 6}, DTPierc, 10, Licensed},
	/*ForcePike*/{AdvancedMeleeWeapons, Medium, 500, {2, 8}, DTPierc|DTEnerg, 20, Restricted},
	/*Electrostaff*/{AdvancedMeleeWeapons, Large, 3000, {2, 6}, DTBludg|DTEnerg, 20, Restricted},
	/*VibroAxe*/{AdvancedMeleeWeapons, Large, 500, {2, 10}, DTSlash, 60, Restricted},
	/*Atlati*/{ExoticWeapon, Medium, 50, {2, 4}, DTBludg, 15, Rare},
	/*Amphistaff*/{ExoticWeapon, Large, 0, {3, 6}, 0, 20, Rare},
	/*Cesta*/{ExoticWeapon, Large, 100, {2, 4}, DTBludg, 18, Rare},
	/*LightSaberShort*/{LightSabers, Small, 2500, {2, 6}, DTEnerg|DTSlash, 5, Rare},
	/*LightSaber*/{LightSabers, Medium, 3000, {2, 8}, DTEnerg|DTSlash, 10, Rare},
	/*LightSaberDouble*/{LightSabers, Large, 7000, {2, 8}, DTEnerg|DTSlash, 20, Rare},
	/*Knife*/{SimpleWeapons, Tiny, 25, {1, 4}, DTPierc|DTSlash|RoFS, 10, AvaibleNormal},
	/*Club*/{SimpleWeapons, Small, 15, {1, 6}, DTBludg, 5, AvaibleNormal},
	/*Stun baton*/{SimpleWeapons, Small, 15, {1, 6}, DTBludg|DTStun, 5, AvaibleNormal},
	/*Mace*/{SimpleWeapons, Medium, 50, {1, 8}, DTBludg, 25, AvaibleNormal},
	/*Spear*/{SimpleWeapons, Medium, 60, {1, 8}, DTPierc, 15, AvaibleNormal},
	/*Bayonet*/{SimpleWeapons, Medium, 50, {1, 8}, DTPierc, 10, AvaibleNormal},
	/*Quaterstaff*/{SimpleWeapons, Large, 50, {1, 6}, DTBludg, 18, AvaibleNormal},
	/*Gloves* light*/{SimpleWeapons, Small, 150, {0, 0, 1}, DTBludg, 4, AvaibleNormal},
	/*Gloves medium*/{SimpleWeapons, Small, 250, {0, 0, 1}, DTBludg, 5, AvaibleNormal},
	//
	/*Flametrower*/{ExoticWeapon, Medium, 1000, {3, 6}, RoFS|DTFire|AttArea, 70, Military},
	/*Bowcaster*/{ExoticWeapon, Large, 1500, {3, 10}, RoFS|DTEnerg|DTPierc, 80, Rare},
	/*Grenade launcher*/{HeavyWeapons, Medium, 500, {0, 0}, RoFS, 50, Military},
	/*Bl.heavy reap*/{HeavyWeapons, Large, 4000, {3, 10}, RoFA|DTEnerg, 120, Military},
	/*Bl.cannon*/{HeavyWeapons, Large, 3000, {3, 12}, RoFS|DTEnerg|AttArea, 180, Military},
	/*missile launcher*/{HeavyWeapons, Large, 1500, {6, 6}, RoFS|DTSlash, 100, Military},
	/*e-web,reap*/{HeavyWeapons, Huge, 8000, {3, 12}, RoFA|DTEnerg, 380, Military},
	/*BlPistolHoldOut*/{Pistols, Tiny, 300, {3, 4}, RoFS|DTEnerg, 5, Illegal},
	/*BlasterPistol*/{Pistols, Small, 500, {3, 6}, RoFS|DTEnerg|DTStun, 10, Restricted},
	/*BlasterPistolSp*/{Pistols, Small, 300, {3, 4}, RoFS|DTEnerg|DTStun, 10, Licensed},
	/*IonPistrol*/{Pistols, Small, 250, {3, 6}, RoFS|DTIon, 10, Licensed},
	/*SlugthrowerPistol*/{Pistols, Small, 250, {2, 6}, RoFS|DTPierc, 14, Licensed},
	/*BlasterPistolHv*/{Pistols, Medium, 750, {3, 8}, RoFS|DTEnerg|DTStun, 13, Military},
	/*BlasterCarbine*/{Rifles, Medium, 900, {3, 8}, RoFS|RoFA|DTEnerg|DTStun, 22, Restricted},
	/*BlasterRifle*/{Rifles, Medium, 1000, {3, 8}, RoFS|RoFA|DTEnerg|DTStun, 45, Restricted},
	/*BlasterRifleSp*/{Rifles, Medium, 800, {3, 6}, RoFS|RoFA|DTEnerg|DTStun, 40, Licensed},
	/*IonRifle*/{Rifles, Medium, 800, {3, 8}, RoFS|DTIon, 31, Restricted},
	/*SlugthrowerRifle*/{Rifles, Medium, 300, {3, 6}, RoFS|RoFA|DTPierc, 40, Restricted},
	/*BlLightRepeat*/{Rifles, Large, 1200, {3, 8}, RoFA|DTEnerg, 60, Military},
	/*BlRifleHeavy*/{Rifles, Large, 2000, {3, 10}, RoFS|RoFA|DTEnerg, 60, Military},
	/*EnergyBall*/{SimpleWeapons, Tiny, 20, {2, 8}, DTEnerg, 2, Licensed},
	/*GrenadeFrag*/{SimpleWeapons, Tiny, 200, {4, 8}, DTSlash, 5, Military},
	/*GrenadeIon*/{SimpleWeapons, Tiny, 250, {4, 8}, DTIon, 5, Restricted},
	/*GrenadeStun*/{SimpleWeapons, Tiny, 250, {4, 8}, DTStun, 5, Restricted},
	/*ThermalDetonator*/{SimpleWeapons, Tiny, 2000, {8, 6}, DTEnerg, 10, Illegal},
	/*Sling*/{SimpleWeapons, Small, 35, {1, 4}, RoFS|DTBludg, 3, AvaibleNormal},
	/*Bow*/{SimpleWeapons, Small, 300, {1, 6}, RoFS|DTBludg, 14, AvaibleNormal},
	/*Net*/{SimpleWeapons, Small, 25, {0, 0}, RoFS, 45, AvaibleNormal},
	//
	/*Explosive charge*/{SimpleWeapons, Tiny, 1500, {10, 6}, DTEnerg, 5, Restricted},
	/*Detonite*/{SimpleWeapons, Tiny, 1500, {10, 6}, DTEnerg, 5, Restricted},
	//
	/*BlastHelmet*/{ArmourLight, Medium, 500, {2, 0, 5}, 0, 30, AvaibleNormal},
	/*FlightSuit*/{ArmourLight, Medium, 2000, {3, 1, 4}, 0, 50, AvaibleNormal},
	/*CombatJumpSuit*/{ArmourLight, Medium, 1500, {4, 0, 4}, 0, 80, Licensed},
	/*FlightSuitArm*/{ArmourLight, Medium, 4000, {5, 2, 3}, 0, 100, Licensed},
	/*VonduunCrabsh*/{ArmourLight, Medium, 0, {5, 5, 4}, 0, 50, Rare},
	/*StormtrooperArm*/{ArmourLight, Medium, 8000, {6, 2, 3}, 0, 100, Military},
	//
	/*CeremonialArmour*/{ArmourMedium, Medium, 5000, {7, 0, 2}, 0, 130, Restricted},
	/*CorelianPowerSui*/{ArmourMedium, Medium, 10000, {7, 0, 3}, 0, 200, Restricted},
	/*BattleArmour*/{ArmourMedium, Medium, 7000, {8, 2, 2}, 0, 160, Military},
	//
	/*ArmouredSpacesui*/{ArmourHeavy, Medium, 12000, {9, 3, 1}, 0, 350, Restricted},
	/*BattleArmourHeav*/{ArmourHeavy, Medium, 15000, {10, 4, 1}, 0, 300, Military},
	//
	/*PortableComputer*/{None, Tiny, 5000, {0, 0, 0}, 0, 20, AvaibleNormal},
	/*CodeCylinder*/{None, Tiny, 500, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*CreditChip*/{None, Tiny, 100, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*Holoprojector*/{None, Tiny, 1000, {0, 0, 0}, 0, 5, AvaibleNormal},
	/*DataCard*/{None, Tiny, 10, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*DataPad*/{None, Tiny, 1000, {0, 0, 0}, 0, 5, AvaibleNormal},
	//
	/*Electrobinocular*/{None, Tiny, 1000, {0, 0, 0}, 0, 10, AvaibleNormal},
	/*GlowRod*/{None, Tiny, 10, {0, 0, 0}, 0, 10, AvaibleNormal},
	/*FusionLatern*/{None, Tiny, 25, {0, 0, 0}, 0, 20, AvaibleNormal},
	/*AudioRecorder*/{None, Tiny, 1000, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*HoloRecorder*/{None, Tiny, 10, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*VideoRecorder*/{None, Tiny, 1000, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*SensorPack*/{None, Tiny, 1500, {0, 0, 0}, 0, 90, AvaibleNormal},
	//
	/*Comnlink*/{None, Tiny, 25, {0, 0, 0}, 0, 1, AvaibleNormal},
	/*PocketScrambler*/{None, Tiny, 400, {0, 0, 0}, 0, 5, AvaibleNormal},
	/*VoxBox*/{None, Tiny, 200, {0, 0, 0}, 0, 1, AvaibleNormal},
};

const char*	gstring(int t); // get string by token

void item::clear()
{
	rmset(this, 0, sizeof(item));
}

void item::create(int t, int count)
{
	clear();
	type = t - Vibrodagger + 1;
	if(get(IsGrouped))
		content = count;
}

bool item::get(int t, wchar_t* text) const
{
	switch(t)
	{
	case Name:
		if(type)
		{
			szcpy(text, gstr(get(Class)));
			if(content>1)
				if(get(IsGrouped))
					szprint(text+szlen(text), L" x%1i", content);
		}
		else
			szcpy(text, szt(L"пусто",L"empthy"));
		break;
	case Description:
		szcpy(text, gdescription(get(Class)));
		break;
	default:
		return false;
	}
	return true;
}

bool item::add(item& it, bool run)
{
	if(*this!=it)
		return false;
	if(!get(IsGrouped))
		return false;
	if(content+it.content>=255)
		return false;
	if(run)
		content += it.content;
	return true;
}

bool item::del(int count, bool run)
{
	if(content-count<0)
		return false;
	content -= count;
	return true;
}

void item::gdamage(damage& Damage) const
{
	damage& d = data[type].Damage;
	if(d.dice)
	{
		Damage.count = d.count;
		Damage.dice = d.dice;
		Damage.multiplier = d.multiplier;
	}
	Damage.modifier += d.modifier;
	Damage.bonus += d.bonus;
}

int	item::get(int t) const
{
	switch(t)
	{
	case IsRanged:
		return (data[type].flags&(RoFA|RoFS))!=0 ? 1 : 0;
	case IsGrouped:
		switch(get(Class))
		{
		case GrenadeFrag:
		case GrenadeIon:
		case GrenadeStun:
			break;
		case BlastHelmetAndVest:
		case FlightSuitPadded:
		case CombatJumpSuit:
		case FlightSuitArmored:
		case VonduunCrabshell:
		case StormtrooperArmour:
		case CeremonialArmour:
		case CorelianPowerSuit:
		case BattleArmour:
		case ArmouredSpacesuit:
		case BattleArmourHeavy:
			return 0;
		default:
			return (data[type].flags&(~(RoFA|RoFS)))==0;
		}
		return 1;
	case Size:
		return data[type].size;
	case Reflexes:
		return data[type].Damage.count;
	case Fortitude:
		return data[type].Damage.dice;
	case Dex:
		return data[type].Damage.modifier;
	case Credits:
		if(get(IsGrouped))
			return data[type].Cost*content;
		return data[type].Cost;
	case Weight:
		if(get(IsGrouped))
			return data[type].weight*content;
		return data[type].weight;
	case License:
		return data[type].available;
	case Class:
		return type+Vibrodagger-1;
	case BaseClass:
		return data[type].Group;
	case PrimaryAbility:
		return get(IsRanged) ? Missile : Melee;
	case Melee:
		switch(get(BaseClass))
		{
		case SimpleWeapons:
		case AdvancedMeleeWeapons:
		case LightSabers:
			return 1;
		default:
			return 0;
		}
		break;
	case Missile:
		switch(get(BaseClass))
		{
		case Pistols:
		case Rifles:
		case HeavyWeapons:
			return 1;
		default:
			return 0;
		}
		break;
	case Armour:
		switch(get(BaseClass))
		{
		case ArmourLight:
		case ArmourMedium:
		case ArmourHeavy:
			return 1;
		default:
			return 0;
		}
		break;
	case Count:
		if(get(IsGrouped))
			return content;
		return 1;
	default:
		return 0;
	}
}

const wchar_t* damage::tostring() const
{
	static wchar_t text[32];
	text[0] = 0;
	if(count)
		szprint(text, L"%1/%2id%3i", ::bonus(bonus), count, dice);
	else
		szcat(text, ::bonus(bonus));
	if(multiplier>1)
		szprint(szend(text), L"x%1i", multiplier);
	if(modifier>1)
		szprint(szend(text), L"+%1i", modifier);
	return text;
}