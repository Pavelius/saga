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

#include "crt.h"

enum locales
{
	LocaleRu, LocaleEn,
	LocaleCount,
};
enum tokens
{
	None,
	// ������ ����������
	Jedi, Noble, Scoundrel, Scout, Soldier, NonHero, Beast,
	AcePilot, BountyHunter, CrimeLord, EliteTrooper, ForceAdept, ForceDisciple, GunSlinger, JediKnight, JediMaster, Officer, SithAppertice, SithLord,
	// ����������� ����������� �������
	AdeptNegotiator, ForcePersuasion, SkilledAdvisor,
	AcrobaticRecovery, BattleMeditation, ElusiveTarget, ForceIntuition, Resilience,
	ClearMind, DarkSideSence, DarkSideScourge, ForceHaze, ResistDarkSide,
	Block, Deflect, LightsaberDefence, LightSaberThrow, RedirectShot,
	// ����������� ����������� ������������
	Presence, DemandSurrender, ImprovedWeakenResolve, WeakenResolve,
	BolsterAlly, IgniteFerver, InspireConfidence, InspireHaste, InspireZeal,
	BornLeader, Coordinate, DistantCommand, FearlessLeader, Rally, Trust,
	Connections, Educated, SpontaneousSkill, Wealth,
	// ����������� ����������� ��������
	FoolsLuck, FortuneFavor, Gambler, Knack, LuckyShoot,
	BastardlyStrike, Disruptive, Skirmisher, SneakAttack, WalkTheLine,
	Gimmick, MasterSlicer, Tracer,
	Hyperdriven, Spacehound, StarshipRaider, StellarWarrior,
	// ����������� ����������� �������
	AcuteSeaser, ExpertTracker, ImprovedInitiative, KeenShot, UncannyDodgeI, UncannyDodgeII,
	HiddenMovement, ImprovedStealth, TotalConcealment,
	Barter, FringeSavant, LongStride, JuryRigger,
	Evasion, ExtremeEffort, Sprint, Surefooted,
	// ����������� ����������� ������
	ArmourMastery, ArmoredDefence, ImprovedArmoredDefence, Juggernaut, SecondSkin,
	ExpertGrappler, GunClub, MeleeSmash, StunningStrike, UnbalanceOpponent,
	BattleAnalysis, CoverFire, Demolitionist, DrawFire, HarmsWay, Indomitable, ThoughtAsNail,
	DevastatingAttack, PenetratingAttack,
	// C���������� ����������� ������� ����
	ElusiveDogFighter, FullThrottle, Juke, KeepItTogether, RelentlessPursuit, VehicularEvasion,
	DogfightGunner, ExpertGunner, QuickTrigger, SystemHit,
	// ����������� ����������� ��������� �� �������
	HuntersMark, HuntersTarget, Notorious, NowhereToHide, Relentless, RushlessNegotiator,
	// ����������� ����������� ������������� ����
	DisciplinedStrike, TelekineticPower, TelekineticSavant,
	DamageReduction10, Equilibrium, ForceFocus, ForceRecovery,
	PowerOfDarkSide, DarkPresence, Revenge, SwiftPower,
	ForcePerception, ForcePilot, Foresight, GaugeForcePotential, Visions,
	// ����� �����������
	AcrobaticStrike, ArmourProficience,
	BanthaRush, BurstFire, CarefulShoot, ChargingFire,
	Cleave, CombatReflexes, CoordinatedAttack, Crush, CyberneticSurgery,
	Deadeye, Dodge, DoubleAttack, DreadfulRage, DualWeaponMastery,
	ExoticWeapon,
	ExtraRage, ExtraSecondWind,
	FarShoot, ForceBoon, ForceSensitivity, ForceTraining,
	ImprovedCharge, ImprovedDefences, ImprovedDisarm, ImprovedDamageThreshold, Linguist,
	MartialArts, MeleeDefence, MightySwing,
	Mobility, Pin, PointBlankShoot, PowerAttack, PowerfulCharge,
	PreciseShoot, QuickDraw, RapidShoot, RapidStrike, RunningAttack,
	ShakeItOff, SkillTraining,
	Sniper, StrongInTheForce,
	SurgicalExpertise, Throw, Toughness, Trip, TripleAttack,
	TripleCrit, VehicularCombat, WeaponFiness,
	WhirlwindAttack,
	// ������
	Acrobatic, Climb, Deception, Endurance, GatherInformation,
	Initiative, Jump, Buerocracy, GalacticLore, LifeSciences,
	PhysicalSciences, SocialSciences, Tactics, Technology, Mechanics,
	Perception, Persuasion, Pilot, Ride, Stealth,
	Survival, Swim, TreatInjury, UseComputer, UseForce,
	FirstSkill = Acrobatic, LastSkill = UseForce,
	// ������ ������
	SimpleWeapons, AdvancedMeleeWeapons, Pistols, Rifles, HeavyWeapons, LightSabers,
	// �������� �����������
	Primitive, LowlightVision, Darkvision,
	ExpertSwimmer, ExpertPilot, ExpertClimber, Sneaky, SurvivalInstinct, IntuitiveInitiative, KeenForceSence, HeightenAwareness, Deceptive, ExtraordinaryRecuperation,
	HoldBreath, BreathUnderwater, Bellow, LimbRegeneration, NaturalArmour,
	IronWill, GreatFortitude, LightingReflexes, SuperiorDefences,
	Rage, Scent,
	// ����������� ����
	BattleStrike, DarkRage, Farseeing, ForceDisarm,
	ForceGrip, ForceLighting, ForceSlam, ForceStun,
	ForceThrust, MindTrick, MoveObject, NegateEnergy,
	Rebuke, SeverForce, Surge, VitalTransfer,
	// ��������
	Str, Dex, Con, Int, Wis, Cha,
	Race, Class, Gender, Hits, HitsRolled, Exp, Credits, Languages,
	// ������ ���������
	DestinyPoints, ForcePoints, DarkSideScore,
	// ����������� ���������� ���������� �������
	VehicleDodge, FamiliarFoe,
	FirstFeat = AdeptNegotiator, LastFeat = FamiliarFoe,
	// ����
	Human, Bothan, Cerean, Duros, Ewoks, Gammorean, Gungans, Ithorians,
	KelDor, MonCalamari, Quarren, Rodians, Sullustans, Txrandoshans, Twileks, Zabrak,
	Wookie,
	FirstSpecies = Human, LastSpecies = Wookie,
	// ���������
	Normal, Helpless,
	// ������
	Skills, SkillsAllowed, SkillsPoints,
	Talents, TalentsAllowed, TalentsPoints,
	Feats, FeatsAllowed, FeatsPoints,
	ForcePowers, ForcePowersAllowed, ForcePowersPoints,
	LanguagesAllowed, LanguagesPoints,
	Items, ItemsAllowed,
	BaseClass, Ability, Size,
	// ���� �����
	ArmourLight, ArmourMedium, ArmourHeavy,
	// ������ ����������
	StrBonus, DexBonus, ConBonus, IntBonus, WisBonus, ChaBonus,
	Reflexes, Fortitude, Will, DamageTreshold, BaB,
	Speed,
	HitsTotal, HitDice,
	Melee, Missile,
	ReflexesFlatFooted,
	Master, HeroicLevel,
	CriticalHit, CriticalFail,
	ForcePointsByLevel,
	Equipment, State, License,
	Weight, EquipmentWeight, EncumbranceWeight,
	EquipmentCost,
	// �������
	Tiny, Small, Medium, Large, Huge, Gigantic,
	// ����������� ��������
	AvaibleNormal, Licensed, Restricted, Military, Illegal, Rare,
	// �����
	Basic, Binary, Bocce, Bothese, CereanLanguage, Dosh, Durese, Ewokese, GammoreanLanguage, Gunganese, HighGalactics,
	Huttese, Ithorese, JawaTradeLanguage, KelDorLanguage, MonCalamarian, Quarreness, Rodese, Ryl, Shyriiwook, Sullustese, ZabrakLanguage,
	FirstLanguage = Basic, LastLanguage = ZabrakLanguage,
	// ��������
	Vibrodagger, Vibroblade, Vibrobayonet, ForcePike, Electrostaff, VibroAxe, Atlati, Amphistaff, Cesta,
	LightSaberShort, LightSaber, LightSaberDouble,
	Knife, Club, StunBatton, Mace, Spear, Bayonet, QuaterStaff, GlovesSmall, GlovesMedium,
	FlameThrower, BowCaster, // Exotic Ranged
	GrenadeLauncher, BlasterHeavyRepeating, BlasterCannon, MissileLauncher, BlasterEWebRepeating, // Heavy weapon
	BlasterPistolHoldOut, BlasterPistol, BlasterPistolSport, IonPistol, SlugthrowerPistol, BlasterPistolHeavy, // Pistols
	BlasterCarbine, BlasterRifle, BlasterRifleSport, IonRifle, SlugthrowerRifle, BlasterLightReapeating, BlasterRifleHeavy, // Rifles
	EnegryBall, GrenadeFrag, GrenadeIon, GrenadeStun, ThermalDetonator,
	Sling, Bow, Net, // Simple ranged
	ExplosiveCharge, Detonite, // Explosives
	BlastHelmetAndVest, FlightSuitPadded, CombatJumpSuit, FlightSuitArmored, VonduunCrabshell, StormtrooperArmour,
	CeremonialArmour, CorelianPowerSuit, BattleArmour,
	ArmouredSpacesuit, BattleArmourHeavy,
	PortableComputer, CodeCylinder, CreditChip, Holoprojector, DataCard, DataPad,
	Electrobinocular, GlowRod, FusionLatern, AudioRecorder, HoloRecorder, VideoRecorder, SensorPack,
	Comlink, PocketScrambler, VoxBox,
	FirstEquip = Vibrodagger, LastEquip = VoxBox,
	// ���
	Male, Female,
	// common property
	Name, Slot1, Slot2, Slot3, Slot4, Slot5, Slot6, Slot7, Slot8,
	NewCharacter,
	Wear, Body, Armour, SavingThrows, Description,
	Classes, ClassesPrestige,
	General, Abilities, PrimaryAbility,
	GeneralStates,
	Setting, SystemLanguage, SystemLanguageEN, SystemLanguageRU, ShowPageNumber, ShowEnglishTranscript,
	CharSheet, Print, Count,
	// commands
	Clear, Generate, OK, Cancel, Choose, ChooseRace, ChooseGender,
	// controls
	SkillsControl = 6600,
	FeatsControl, FeatsAllowedControl,
	TalentsControl, TalentsAllowedControl,
	ForcePowersControl, ForcePowersAllowedControl,
	LanguagesControl, LanguagesAllowedControl,
	ItemsControl, ItemsAllowedControl,
	IsGrouped, IsRanged, Tabs, EmpthySlot,
	Increment, Decrement, Number, NumberC, Text, Bonus,
};
enum action_s
{
	AcNO,				// �� ������� ������
	AcST,				// ����������� ��������
	AcMV,				// �������� ��������
	AcSW,				// ��������������� ��������
	AcS2,				// 2 ��������������� ��������
	AcS3,				// 3 ��������������� ��������
	AcRA,				// �������
	AcFR,				// �������� �� ������ �����
	AcFE,				// ��������� ��������
	Ac1H,				// ��� �������
};
struct damage
{
	char				count;
	char				dice;
	char				modifier;
	char				multiplier;
	char				bonus;
	//
	const wchar_t*		tostring() const;
	void				clear() { count = 0; dice = 0; modifier = 0; multiplier = 1; bonus = 0; }
};
struct item
{
	unsigned char		stealed : 1;	// ������� ��� �������
	unsigned char		reserved : 7;	// ����������� �������
	unsigned char		type;			// ��� ��������:  [+Vibrodagger-1]. 0 - ������� �� ��������
	unsigned char		effect;			// ������: [+Vibrodagger-1]. 0 - ��� �����������
	unsigned char		content;		// ���������� - ���������� ��������, ��������, ����� ���� 0 ��� ������ � ��������� ��� ����������
	//
	bool				operator==(const item& i) { return type==i.type && effect==i.effect; }
	bool				operator!=(const item& i) { return type!=i.type || effect!=i.effect; }
	//
	bool				add(item& Item, bool run);
	void				clear();
	void				create(int t, int count=1);
	bool				del(int count, bool run);
	bool				isvalid() const { return type!=0; }
	int					get(int t) const;
	bool				get(int t, wchar_t* text) const;
	void				gdamage(damage& Damage) const;
};
struct wear
{
	item				armour;
	item				melee;
	item				second;
	item				ranged;
};
struct unit
{
	int					attr[DarkSideScore+1];
	wchar_t				name[128];
	item				posses[8];
	wear				wears;
	//
	void				add(int experience, bool auto_level_up); // �������� ����� �����
	bool				add(item it, bool run); // �������� ������� i
	bool				allow(int f, int m) const; // ��������� �� ����������� f �� ������ m ���������
	bool				allow(item it) const;
	static int			compare(const void* c1, const void* c2);
	bool				del(item it, bool run);
	bool				equip(item* Item, bool run);
	inline int			gbonus(int i) const { return get(i)/2-5; }
	int					get(int i) const;
	void				get(item it, wchar_t* text) const; // �������� �������� � ������ ��������� �����
	bool				get(int token, wchar_t* string) const;
	void				get(int token, wchar_t* string, int m, int n) const;
	int					get(int* data, int id, bool weared) const;
	void				gdamage(item i, damage& d, int boost=0) const;
	const item*			gitem(int slot) const;
	void				levelup(int type);
	void				sort(int* data, unsigned count, bool weared = false) const;
	void				random(int race, int clas, int gender, int exp);
	int					roll(int skill, int dice, int modifier, int& critical) const;
	bool				set(int token, const int v);
	bool				set(int token, const wchar_t* v);
private:
	int					gattrc(const tokens t1, const tokens t2, bool PresenceOnly, int Level = 1) const;
	int					gattrcmax(const tokens t1, const tokens t2, int MaxLevel) const;
	int					gattrcbs(const tokens t1, const tokens t2) const;
	int					gbonusfort() const;
	int					gbonusref(bool flat_footed) const;
	int					gclassc() const;
};
namespace setting
{
	namespace show
	{
		extern bool		transcript;
		extern bool		pages;
	}
}
namespace feats
{
	tokens				ability(int id);
	action_s			action(const int id);
	void				fill(int* array, int e1, int e2);
	const wchar_t*		gets(int id, int level);
	const wchar_t*		getsen(int id, int level);
	const wchar_t*		info(int id, int level);
	int					page(int id);
	int					size(const int* data);
	int					sort(const void* v1, const void* v2, void* param);
};

const wchar_t*			bonus(int num, const wchar_t* zero=0);
int						gability(int t);
int						gtkpage(int t);
const wchar_t*			gdescription(int t);
const wchar_t*			gstr(int t);
const wchar_t*			gstren(int id);
const wchar_t*			gtag(int i);
const wchar_t*			number(int num, bool bracers = false, const wchar_t* suffix=0);
int						skill_ability(int i);
const wchar_t*			upper(const wchar_t* string);
const wchar_t*			weight(int num);