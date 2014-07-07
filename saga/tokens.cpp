//   Copyright 2013 by Pavel Chistyakov
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

struct token
{
	const wchar_t* name[LocaleCount];
	const wchar_t* description;
};

static token data[] = {{{L"человек", L"human"}, L"Люди составляют подавляющее большинство населения галактики."},
{{L"ботан", L"bothan"}, L"Планета Ботауи в Среднем Кольце галактики дала жизнь расе Ботанов."},
{{L"цереан", L"cerean"}, L"Это мирная раса, они занимаются земледелием, и их традиции насчитывают многие тысячи лет."},
{{L"дурос", L"duros"}, L"У Дуросов врожденная склонность к космическим ппутешествиям."},
{{L"эвок", L"ewok"}, L"Эвоки живут на одной из лун планеты Эндор, которая покрыта густыми лесами."},
{{L"гамореанец", L"gammorean"}, L"Жестокие и яростные Гаморреанцы происходят с планеты Гаморр, находящейся во Внешнем Кольце."},
{{L"гунган", L"gungan"}, L"Гунганы - древняя и воинственная раса, живущая в глубинах морей и океанов планеты Набу."},
{{L"иторианец", L"ithorian"}, L"Иторианцы живут на покрытой джунглями планете Итор и многочисленных космических станциях."},
{{L"келдорианец", L"kel dor"}, L"Раса Кел Дор развивалась на планете Дорин, атмосфера которой состоит из гелия и еще одного уникального для этой планеты газа."},
{{L"мон каламарец", L"mon calamari"}, L"Вежливая, но могущественная раса Мон Каламари - лучшие конструкторы кораблей в галактике."},
{{L"кваррен", L"quarren"}, L"Кваррены живут на одной покрытой океаном планете с расой Мон Каламари."},
{{L"родианец", L"rodian"}, L"Культура Родианцев основана на традициях охоты и следопытства."},
{{L"суллустанец", L"sulustan"}, L"Суллустанцы живут в огромных пещерных городах под поверхностью планеты Салласт - сурового вулканического мира, окутанного густыми облаками ядовитого газа."},
{{L"трандошанец", L"trandoshan"}, L"Похожие на ящеров Трандошанцы известны своей огромной силой и воинственным характером."},
{{L"тви\'лек", L"twilek"}, L"Раса Тви\'леков появилась на сухой каменистой планете Рилот и распространилась по всей галатике."},
{{L"забрак", L"zabrak"}, L"Забраки вышли в космос очень давно, и у них очень много колоний."},
{{L"вуки", L"wookie"}, L"Известные своими боевыми навыками, Вуки очень сильны, долго живут и склонны к техническим открытиям."},
// Состояние
{{L"нормальное", L"normal"}, L""},
{{L"безпомощен", L"helpless"}, L""},
// Кнокпи
{{L"навыки", L"skills"}, L""},
{{L"доступно навыков", L"skills available"}, L""},
{{L"очков навыков осталось", L"skills points available"}, L""},
{{L"таланты", L"talents"}, L""},
{{L"доступно талантов", L"talents available"}, L""},
{{L"очков талантов осталось", L"talents points available"}, L""},
{{L"особенности", L"feats"}, L""},
{{L"доступные особенности", L"feats available"}, L""},
{{L"очков особенностей осталось", L"feats points available"}, L""},
{{L"сила", L"force points"}, L""},
{{L"сила доступно", L"force points available"}, L""},
{{L"очков силы осталось", L"force points available"}, L""},
{{L"языки доступные", L"languages available"}, L""},
{{L"свободных языков осталось", L"free languages available"}, L""},
{{L"экипировка", L"equipment"}, L""},
{{L"экипировка доступная", L"equipment Allowed"}, L""},
{{L"базовый класс", L"base class"}, L""},
{{L"способности", L"ability"}, L""},
{{L"размер", L"size"}, L""},
// Виды брони
{{L"легкая броня", L"light armour"}, L""},
{{L"средняя броня", L"medium armour"}, L""},
{{L"тяжелая броня", L"heavy armour"}, L""},
// Боевые показатели
{{L"бонус силы", L"strenght bonus"}, L"Добавляется ко всем броска, где используется сила. Считается как значение силы/2 - 5."},
{{L"бонус ловкости", L"dexterity bonus"}, L"Добавляется ко всем броска, где используется ловкость. Считается как значение ловкости/2 - 5."},
{{L"бонус телосложения", L"constitution bonus"}, L"Добавляется ко всем броска, где используется телосложение. Считается как значение телосложения/2 - 5."},
{{L"бонус интеллекта", L"intellegence bonus"}, L"Добавляется ко всем броска, где используется интеллект. Считается как значение интеллекта/2 - 5."},
{{L"бонус мудрости", L"wisdow bonus"}, L"Добавляется ко всем броска, где используется мудрость. Считается как значение мудрости/2 - 5."},
{{L"бонус харизмы", L"charisma bonus"}, L"Добавляется ко всем броска, где используется харизма. Считается как значение харизмы/2 - 5."},
{{L"рефлексы", L"reflexes"}, L""},
{{L"стойкость", L"fortitude"}, L""},
{{L"воля", L"will"}, L""},
{{L"болевой порог", L"damage threshold"}, L"Если получаемый урон превышает болевой порог уменьшите состояние на единицу."},
{{L"базовая атака", L"base attack"}, L""},
{{L"скорость", L"speed"}, L""},
{{L"максимум хитов", L"hits totals"}, L""},
{{L"кубик хитов", L"hit dices"}, L""},
{{L"ближний бой", L"melee"}, L""},
{{L"дистанция", L"range"}, L""},
{{L"рефлексы", L"reflexes"}, L""},
{{L"хозяин", L"owner"}, L""},
{{L"уровень", L"level"}, L""},
{{L"критический успех", L"critical succes"}, L""},
{{L"критический промах", L"critical miss"}, L""},
{{L"очки силы", L"force points by level"}, L""},
{{L"оборудование", L"equipment"}, L""},
{{L"состояние", L"state"}, L""},
{{L"лицензия", L"license"}, L""},
{{L"вес", L"weight"}, L"Вес, измеряемый в килограммах."},
{{L"вес всего оборудования", L"equipment weight"}, L"Вес всего оборудования находящегося у персонажа в кредитах, включая броню, которая надета на него и оружие, которое он держит в руках."},
{{L"максимально возможный вес", L"maximum encumbrance"}, L""},
{{L"стоимость всего оборудования (cr)", L"equipment cost totals"}, L"Стоимость всего оборудования находящегося у персонажа в кредитах."},
// Размеры
{{L"крошечный", L"diminutive"}, L""},
{{L"малый", L"small"}, L""},
{{L"средний", L"medium"}, L""},
{{L"большой", L"large"}, L""},
{{L"огромный", L"huge"}, L""},
{{L"гиганский", L"collosal"}, L""},
// Доступность
{{L"нет", L"no"}, L""},
{{L"по лицензии", L"licensed"}, L""},
{{L"ограничено", L"strict"}, L""},
{{L"военная", L"military"}, L""},
{{L"запрещенное", L"restricted"}, L""},
{{L"редкое", L"rare"}, L""},
// Языки
{{L"базовый", L"basic"}, L"базовом языке"},
{{L"бинарный", L"binary"}, L"бинарном языке"},
{{L"бокке", L"bocce"}, L"бокке"},
{{L"ботесс", L"bothese"}, L"ботессе"},
{{L"цереан", L"cerean"}, L"цереаннском языке"},
{{L"дош", L"dosh"}, L"доше"},
{{L"дюрес", L"durese"}, L"дюресе"},
{{L"эвокский", L"ewokese"}, L"эвокском языке"},
{{L"гаморианский", L"gamorrean"}, L"гамморианском языке"},
{{L"гунганский", L"gunganese"}, L"гунганском языке"},
{{L"высший галактический", L"hight galactic"}, L"высшем галактическом языке"},
{{L"хатский", L"huttese"}, L"языке хаттов"},
{{L"иторианский", L"ithorese"}, L"иторианском языке"},
{{L"торговый язык джавов", L"jawa trade"}, L"джавском торговом языке"},
{{L"келдорианский", L"kel dor"}, L"келдорианском языке"},
{{L"монкаламарский", L"mon calamarian"}, L"монкаламарском языке"},
{{L"куаринский", L"quarrenese"}, L"куаринском языке"},
{{L"родесс", L"rodese"}, L"родессе"},
{{L"рил", L"ryl"}, L"рилле"},
{{L"ширивуук", L"shyriiwook"}, L"ширивууке"},
{{L"сулустанский", L"sulustese"}, L"сулустанском языке"},
{{L"забрак", L"zabrak"}, L"забраке"},
//
{{L"вибро-кинжал", L"vibro dagger"}, L""},
{{L"вибро-меч", L"vibro blade"}, L""},
{{L"вибро-штык", L"vibro boatan"}, L""},
{{L"силовое копье", L"power spear"}, L""},
{{L"электро-посох", L"power staff"}, L""},
{{L"вибро-топор", L"vibro axe"}, L""},
{{L"атлати", L"atlati"}, L""},
{{L"эмфи-посох", L"emfy staff"}, L""},
{{L"цеста", L"cesta"}, L""},
{{L"световой кинжал", L"light dagger"}, L""},
{{L"световой меч", L"light saber"}, L""},
{{L"световой меч, двойной", L"light saber, double"}, L""},
// Простое оружие
{{L"нож", L"knife"}, L""},
{{L"дубина", L"club"}, L""},
{{L"шокирующая палка", L"baton"}, L""},
{{L"булава", L"mace"}, L""},
{{L"копье", L"spear"}, L""},
{{L"штык на винтовку", L"boatan"}, L""},
{{L"посох", L"staff"}, L""},
{{L"перчатки", L"gauntlets"}, L""},
{{L"перчатки", L"gauntlets"}, L""},
// Дистанционное оружие
{{L"огнемет", L"firebat"}, L""},
{{L"плазмолет", L"plasmogun"}, L""},
{{L"гранатомет", L"granade launcher"}, L""},
{{L"тяжелый лазерный пулемет", L"blaster machinegun"}, L""},
{{L"лазерная пушка", L"blaster cannon"}, L""},
{{L"ракетница", L"rocket launcher"}, L""},
{{L"лазерная установка E-Web", L"blaster turrel E-web"}, L""},
{{L"мини-бластер", L"mini blaster"}, L""},
{{L"бластер", L"blaster"}, L""},
{{L"спортивный бластер", L"sport blaster"}, L""},
{{L"импульсный бластер", L"ion blaster"}, L""},
{{L"пистолет", L"gun"}, L""},
{{L"тяжелый бластер", L"heavy blaster"}, L""},
{{L"лазерный карабин", L"blaster shotgun"}, L""},
{{L"лазерная винтовка", L"blaster rifle"}, L""},
{{L"спортивная лазерная винтовка", L"sport blaster rifle"}, L""},
{{L"импульсная винтовка", L"ion rifle"}, L""},
{{L"винтовка", L"rifle"}, L""},
{{L"лазерный пулемет", L"blaster minigun"}, L""},
{{L"тяжелая лазерная винтовка", L"heavy blaster rifle"}, L""},
{{L"плазменный шар", L"plasma sphere"}, L""},
{{L"осколочная граната", L"grenade"}, L""},
{{L"импульсная граната", L"grenade, ion"}, L""},
{{L"шокирующая граната", L"grenade, shock"}, L""},
{{L"термальный детонатор", L"thermal detonator"}, L""},
{{L"праща", L"sling"}, L""},
{{L"лук", L"bow"}, L""},
{{L"сеть", L"net"}, L""},
// Взрывчатки
{{L"взрывчатка", L"trotil"}, L""},
{{L"детонатор", L"detonator"}, L""},
// Легкая броня
{{L"шлем и куртка", L"blast helmet and vest"}, L""},
{{L"костюм пилота", L"flight suit"}, L""},
{{L"броня с прыжковым двигателем", L"armour, jumping"}, L""},
{{L"костюм пилота, бронированный", L"flight suit, armoured"}, L""},
{{L"вондуунская крабовая броня", L"vondoon crabshell armour"}, L""},
{{L"броня штурмовика", L"armour, trooper"}, L""},
// Средняя броня
{{L"церемониальная броня", L"ceremonial armour"}, L""},
{{L"энерго-броня", L"power suit"}, L""},
{{L"боевая броня", L"combat armour"}, L""},
// Тяжелая броня
{{L"бронированный космический скафандр", L"space suit, armored"}, L""},
{{L"тяжелая боевая броня", L"combat armour, heavy"}, L""},
// Устройства
{{L"переносной коспьютер", L"portable computer"}, L""},
{{L"закодированный цилиндр", L"code cylinder"}, L""},
{{L"кредитная карточка", L"credit chip"}, L""},
{{L"холопроэктор", L"holoproject"}, L""},
{{L"флешка", L"data card"}, L""},
{{L"карманный компьютер", L"data pad"}, L""},
// Устройства обнаружения
{{L"электро-бинокль", L"electro binocular"}, L"Бинокль позволяет получить информацию о расстоянии, азимуте, текущей радиации, а также оснащены приборами ночного видения. Уменьшает штраф расстояния до -1 за каждые 10 квадратов, вместо -5 за каждые 10 квадратов."},
{{L"светящаяся шашка", L"glow rod"}, L"Переносной осветитель, который производит света, достаточно для освещения 6 квадратов."},
{{L"атомный фонарик", L"fusion latern"}, L"Фонарик обеспечивает свет и тепло. Освещает 6 квадратов впереди."},
{{L"диктофон", L"audio-recorder"}, L"Хранит 100 часов звука в отличном качестве."},
{{L"холорекордер", L"holo-recorder"}, L"Хранит 1 час качественной голограммы."},
{{L"видеокамера", L"video-recorder"}, L"Хранит 10 часов видео в отличном качестве."},
{{L"портативный радар", L"sensor pack"}, L"Квардратная коробка с дисплеем, разноцветными лампочками и индикатором. Позволяет сканировать местность в пределах 1 киллометра на наличие форм жизни, радиосигналов, радиации или других угроз. Использование занимает стандартное действие и дает +5 к броску до следующего хода."},
// Связь
{{L"комлинк", L"comlink"}, L""},
{{L"карманный шифратор", L"pocket decipher"}, L"Простое устройство, которое может быть подключено к передатчикам и приемникам информации, таким как комлинк."},
{{L"голосовой ящик", L"voxbox"}, L""},
// Пол
{{L"мужчина", L"male"}, L""},
{{L"женщина", L"female"}, L""},
//
{{L"имя", L"name"}, L""},
{{L"ячейка 1", L"slot 1"}, L""},
{{L"ячейка 2", L"slot 2"}, L""},
{{L"ячейка 3", L"slot 3"}, L""},
{{L"ячейка 4", L"slot 4"}, L""},
{{L"ячейка 5", L"slot 5"}, L""},
{{L"ячейка 6", L"slot 6"}, L""},
{{L"ячейка 7", L"slot 7"}, L""},
{{L"ячейка 8", L"slot 8"}, L""},
{{L"новый персонаж", L"new character"}, L""},
//
{{L"носит", L"wear"}, L""},
{{L"тело", L"body"}, L""},
{{L"броня", L"armour"}, L""},
{{L"спас-броски", L"saving throws"}, L""},
{{L"описание", L"description"}, L""},
//
{{L"классы", L"classes"}, L""},
{{L"престижные классы", L"prestige classes"}, L""},
//
{{L"общие", L"general"}, L""},
{{L"атрибуты", L"abilities"}, L""},
{{L"основные атрибуты", L"primary ability"}, L""},
//
{{L"основные состояния", L"general states"}, L""},
//
{{L"настройки", L"settings"}, L""},
{{L"язык програмы", L"programm language"}, L""},
{{L"английский", L"english"}, L""},
{{L"русский", L"russian"}, L""},
{{L"показывать номера страниц", L"show page numbers"}, L""},
{{L"показывать английскую транскрипцию", L"show english transcript"}, L""},
{{L"лист", L"sheet"}, L""},
{{L"печать", L"print"}, L""},
{{L"количество", L"count"}, L""},
//
{{L"очистить", L"clear"}, L""},
{{L"перебросить", L"generate"}, L""},
{{L"OK", L"OK"}, L""},
{{L"отмена", L"cancel"}, L""},
{{L"выбрать", L"choose"}, L""},
{{L"выбрать расу", L"choose race"}, L""},
{{L"выбрать пол", L"choose gender"}, L""},
};

const wchar_t* gstr(int id)
{
	if(id<=LastFeat)
		return feats::gets(id, 1);
	if(id>=Human && id<(int)(Human + sizeof(data)/sizeof(data[0])))
		return data[id-Human].name[locale];
	return szt(L"Неизвестно", L"Unknown");
}

const wchar_t* gstren(int id)
{
	if(id<=LastFeat)
		return feats::getsen(id, 1);
	if(id>=Human && id<(int)(Human + sizeof(data)/sizeof(data[0])))
		return data[id-Human].name[LocaleEn];
	return L"Unknown";
}

const wchar_t*	gdescription(int id)
{
	if(id<=FamiliarFoe)
		return feats::info(id,1);
	if(id>=Human && id<=PrimaryAbility)
		return data[id-Human].description;
	return szt(L"Нет описания", L"No description");
}

const wchar_t* gtag(int id)
{
	static wchar_t temp[128];
	wchar_t* pz = temp;
	bool u = true;
	for(const wchar_t* p = gstren(id); *p; p++)
	{
		wchar_t sym = *p;
		switch(sym)
		{
		case ' ':
		case '\t':
		case '-':
		case '_':
		case '.':
		case ',':
		case ':':
			u = true;
			break;
		default:
			*pz++ = u ? chupper(sym) : sym;
			u = false;
			break;
		}
	}
	*pz++ = 0;
	return temp;
}