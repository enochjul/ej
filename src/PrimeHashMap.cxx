//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <stdint.h>

#include "PrimeHashMap.h"

namespace ej {

#ifdef __LP64__

const uint64_t PrimeHashMapPrimes1_5x[] = {
	UINT64_C(3), UINT64_C(6148914691236517205),
	UINT64_C(7), UINT64_C(2635249153387078802),
	UINT64_C(13), UINT64_C(1418980313362273201),
	UINT64_C(23), UINT64_C(802032351030850070),
	UINT64_C(37), UINT64_C(498560650640798692),
	UINT64_C(59), UINT64_C(312656679215416129),
	UINT64_C(89), UINT64_C(207266787345051141),
	UINT64_C(139), UINT64_C(132710389019493177),
	UINT64_C(211), UINT64_C(87425327363552377),
	UINT64_C(331), UINT64_C(55730344633563600),
	UINT64_C(499), UINT64_C(36967422993405915),
	UINT64_C(751), UINT64_C(24562908220651866),
	UINT64_C(1129), UINT64_C(16339011579902171),
	UINT64_C(1723), UINT64_C(10706177639993935),
	UINT64_C(2591), UINT64_C(7119546149637032),
	UINT64_C(3889), UINT64_C(4743312952869517),
	UINT64_C(5851), UINT64_C(3152750653513852),
	UINT64_C(8783), UINT64_C(2100278273222082),
	UINT64_C(13187), UINT64_C(1398858275097410),
	UINT64_C(19819), UINT64_C(930760586997807),
	UINT64_C(29761), UINT64_C(619829443691729),
	UINT64_C(44657), UINT64_C(413076204709442),
	UINT64_C(67021), UINT64_C(275238269702176),
	UINT64_C(100549), UINT64_C(183460243997548),
	UINT64_C(150833), UINT64_C(122299126011612),
	UINT64_C(226267), UINT64_C(81526444747619),
	UINT64_C(339413), UINT64_C(54348961512109),
	UINT64_C(509149), UINT64_C(36230541695475),
	UINT64_C(763771), UINT64_C(24152192311189),
	UINT64_C(1145659), UINT64_C(16101426404985),
	UINT64_C(1718503), UINT64_C(10734193698649),
	UINT64_C(2577791), UINT64_C(7156027805865),
	UINT64_C(3866719), UINT64_C(4770645106021),
	UINT64_C(5800079), UINT64_C(3180429796509),
	UINT64_C(8700193), UINT64_C(2120268374932),
	UINT64_C(13050329), UINT64_C(1413507971615),
	UINT64_C(19575527), UINT64_C(942337035100),
	UINT64_C(29363291), UINT64_C(628224679369),
	UINT64_C(44044967), UINT64_C(418816162893),
	UINT64_C(66067451), UINT64_C(279210773149),
	UINT64_C(99101221), UINT64_C(186140431849),
	UINT64_C(148651847), UINT64_C(124093608293),
	UINT64_C(222977773), UINT64_C(82729071268),
	UINT64_C(334466669), UINT64_C(55152712612),
	UINT64_C(501700033), UINT64_C(36768472912),
	UINT64_C(752550089), UINT64_C(24512313988),
	UINT64_C(1128825151), UINT64_C(16341542405),
	UINT64_C(1693237739), UINT64_C(10894361523),
	UINT64_C(2539856663), UINT64_C(7262907526),
	UINT64_C(3809785009), UINT64_C(4841938332),
	UINT64_C(5714677537), UINT64_C(3227958875),
	UINT64_C(8572016357), UINT64_C(2151972570),
	UINT64_C(12858024541), UINT64_C(1434648379),
	UINT64_C(19287036833), UINT64_C(956432252),
	UINT64_C(28930555271), UINT64_C(637621500),
	UINT64_C(43395832919), UINT64_C(425081000),
	UINT64_C(65093749379), UINT64_C(283387333),
	UINT64_C(97640624069), UINT64_C(188924889),
	UINT64_C(146460936143), UINT64_C(125949926),
	UINT64_C(219691404223), UINT64_C(83966617),
	UINT64_C(329537106341), UINT64_C(55977744),
	UINT64_C(494305659623), UINT64_C(37318496),
	UINT64_C(741458489533), UINT64_C(24878997),
	UINT64_C(1112187734323), UINT64_C(16585998),
	UINT64_C(1668281601491), UINT64_C(11057332),
	UINT64_C(2502422402279), UINT64_C(7371554),
	UINT64_C(3753633603431), UINT64_C(4914369),
	UINT64_C(5630450405227), UINT64_C(3276246),
	UINT64_C(8445675607973), UINT64_C(2184164),
	UINT64_C(12668513411993), UINT64_C(1456109),
	UINT64_C(19002770118047), UINT64_C(970739),
	UINT64_C(28504155177119), UINT64_C(647159),
	UINT64_C(42756232765733), UINT64_C(431439),
	UINT64_C(64134349148663), UINT64_C(287626),
	UINT64_C(96201523723013), UINT64_C(191751),
	UINT64_C(144302285584529), UINT64_C(127834),
	UINT64_C(216453428376797), UINT64_C(85222),
	UINT64_C(324680142565199), UINT64_C(56815),
	UINT64_C(487020213847823), UINT64_C(37876),
	UINT64_C(730530320771747), UINT64_C(25251),
	UINT64_C(1095795481157653), UINT64_C(16834),
	UINT64_C(1643693221736509), UINT64_C(11222),
	UINT64_C(2465539832604767), UINT64_C(7481),
	UINT64_C(3698309748907211), UINT64_C(4987),
	UINT64_C(5547464623360819), UINT64_C(3325),
	UINT64_C(8321196935041231), UINT64_C(2216),
	UINT64_C(12481795402561873), UINT64_C(1477),
	UINT64_C(18722693103842839), UINT64_C(985),
	UINT64_C(28084039655764327), UINT64_C(656),
	UINT64_C(42126059483646511), UINT64_C(437),
	UINT64_C(63189089225469887), UINT64_C(291),
	UINT64_C(94783633838204887), UINT64_C(194),
	UINT64_C(142175450757307483), UINT64_C(129),
	UINT64_C(213263176135961297), UINT64_C(86),
	UINT64_C(319894764203941973), UINT64_C(57),
	UINT64_C(479842146305913151), UINT64_C(38),
	UINT64_C(719763219458869759), UINT64_C(25),
	UINT64_C(1079644829188304641), UINT64_C(17),
	UINT64_C(1619467243782457063), UINT64_C(11),
	UINT64_C(2429200865673685619), UINT64_C(7),
	UINT64_C(3643801298510528509), UINT64_C(5),
	UINT64_C(5465701947765792847), UINT64_C(3),
	UINT64_C(8198552921648689459), UINT64_C(2),
	UINT64_C(12297829382473034303), UINT64_C(1),
	UINT64_C(18446744073709551557), UINT64_C(1),
};

const uint64_t PrimeHashMapPrimes2x[] = {
	UINT64_C(3), UINT64_C(6148914691236517205),
	UINT64_C(7), UINT64_C(2635249153387078802),
	UINT64_C(13), UINT64_C(1418980313362273201),
	UINT64_C(31), UINT64_C(595056260442243600),
	UINT64_C(61), UINT64_C(302405640552615600),
	UINT64_C(127), UINT64_C(145249953336295682),
	UINT64_C(251), UINT64_C(73493004277727297),
	UINT64_C(509), UINT64_C(36241147492553146),
	UINT64_C(1021), UINT64_C(18067330140753723),
	UINT64_C(2039), UINT64_C(9046956387302379),
	UINT64_C(4093), UINT64_C(4506900579943696),
	UINT64_C(8191), UINT64_C(2252074725150720),
	UINT64_C(16381), UINT64_C(1126106103028481),
	UINT64_C(32749), UINT64_C(563276560313583),
	UINT64_C(65521), UINT64_C(281539415969071),
	UINT64_C(131071), UINT64_C(140738562105344),
	UINT64_C(262139), UINT64_C(70370086380544),
	UINT64_C(524287), UINT64_C(35184439197824),
	UINT64_C(1048573), UINT64_C(17592236376208),
	UINT64_C(2097143), UINT64_C(8796130771106),
	UINT64_C(4194301), UINT64_C(4398049656834),
	UINT64_C(8388593), UINT64_C(2199027187719),
	UINT64_C(16777213), UINT64_C(1099511824384),
	UINT64_C(33554393), UINT64_C(549756452864),
	UINT64_C(67108859), UINT64_C(274877927424),
	UINT64_C(134217689), UINT64_C(137438993408),
	UINT64_C(268435399), UINT64_C(68719491328),
	UINT64_C(536870909), UINT64_C(34359738560),
	UINT64_C(1073741789), UINT64_C(17179869744),
	UINT64_C(2147483647), UINT64_C(8589934596),
	UINT64_C(4294967291), UINT64_C(4294967301),
	UINT64_C(8589934583), UINT64_C(2147483650),
	UINT64_C(17179869143), UINT64_C(1073741826),
	UINT64_C(34359738337), UINT64_C(536870912),
	UINT64_C(68719476731), UINT64_C(268435456),
	UINT64_C(137438953447), UINT64_C(134217728),
	UINT64_C(274877906899), UINT64_C(67108864),
	UINT64_C(549755813881), UINT64_C(33554432),
	UINT64_C(1099511627689), UINT64_C(16777216),
	UINT64_C(2199023255531), UINT64_C(8388608),
	UINT64_C(4398046511093), UINT64_C(4194304),
	UINT64_C(8796093022151), UINT64_C(2097152),
	UINT64_C(17592186044399), UINT64_C(1048576),
	UINT64_C(35184372088777), UINT64_C(524288),
	UINT64_C(70368744177643), UINT64_C(262144),
	UINT64_C(140737488355213), UINT64_C(131072),
	UINT64_C(281474976710597), UINT64_C(65536),
	UINT64_C(562949953421231), UINT64_C(32768),
	UINT64_C(1125899906842597), UINT64_C(16384),
	UINT64_C(2251799813685119), UINT64_C(8192),
	UINT64_C(4503599627370449), UINT64_C(4096),
	UINT64_C(9007199254740881), UINT64_C(2048),
	UINT64_C(18014398509481951), UINT64_C(1024),
	UINT64_C(36028797018963913), UINT64_C(512),
	UINT64_C(72057594037927931), UINT64_C(256),
	UINT64_C(144115188075855859), UINT64_C(128),
	UINT64_C(288230376151711717), UINT64_C(64),
	UINT64_C(576460752303423433), UINT64_C(32),
	UINT64_C(1152921504606846883), UINT64_C(16),
	UINT64_C(2305843009213693951), UINT64_C(8),
	UINT64_C(4611686018427387847), UINT64_C(4),
	UINT64_C(9223372036854775783), UINT64_C(2),
	UINT64_C(18446744073709551557), UINT64_C(1),
};

#else

const uint32_t PrimeHashMapPrimes1_5x[] = {
	UINT32_C(3), UINT32_C(1431655765),
	UINT32_C(7), UINT32_C(613566756),
	UINT32_C(11), UINT32_C(390451572),
	UINT32_C(19), UINT32_C(226050910),
	UINT32_C(31), UINT32_C(138547332),
	UINT32_C(47), UINT32_C(91382282),
	UINT32_C(71), UINT32_C(60492497),
	UINT32_C(109), UINT32_C(39403369),
	UINT32_C(167), UINT32_C(25718367),
	UINT32_C(251), UINT32_C(17111423),
	UINT32_C(379), UINT32_C(11332367),
	UINT32_C(571), UINT32_C(7521834),
	UINT32_C(863), UINT32_C(4976787),
	UINT32_C(1297), UINT32_C(3311462),
	UINT32_C(1951), UINT32_C(2201418),
	UINT32_C(2939), UINT32_C(1461370),
	UINT32_C(4409), UINT32_C(974136),
	UINT32_C(6619), UINT32_C(648884),
	UINT32_C(9941), UINT32_C(432045),
	UINT32_C(14923), UINT32_C(287808),
	UINT32_C(22391), UINT32_C(191816),
	UINT32_C(33587), UINT32_C(127875),
	UINT32_C(50383), UINT32_C(85246),
	UINT32_C(75577), UINT32_C(56829),
	UINT32_C(113371), UINT32_C(37884),
	UINT32_C(170063), UINT32_C(25255),
	UINT32_C(255107), UINT32_C(16835),
	UINT32_C(382663), UINT32_C(11223),
	UINT32_C(574003), UINT32_C(7482),
	UINT32_C(861043), UINT32_C(4988),
	UINT32_C(1291579), UINT32_C(3325),
	UINT32_C(1937389), UINT32_C(2216),
	UINT32_C(2906089), UINT32_C(1477),
	UINT32_C(4359169), UINT32_C(985),
	UINT32_C(6538789), UINT32_C(656),
	UINT32_C(9808187), UINT32_C(437),
	UINT32_C(14712311), UINT32_C(291),
	UINT32_C(22068509), UINT32_C(194),
	UINT32_C(33102767), UINT32_C(129),
	UINT32_C(49654151), UINT32_C(86),
	UINT32_C(74481247), UINT32_C(57),
	UINT32_C(111721921), UINT32_C(38),
	UINT32_C(167582897), UINT32_C(25),
	UINT32_C(251374367), UINT32_C(17),
	UINT32_C(377061551), UINT32_C(11),
	UINT32_C(565592371), UINT32_C(7),
	UINT32_C(848388571), UINT32_C(5),
	UINT32_C(1272582881), UINT32_C(3),
	UINT32_C(1908874333), UINT32_C(2),
	UINT32_C(2863311511), UINT32_C(1),
	UINT32_C(4294967291), UINT32_C(1),
};

const uint32_t PrimeHashMapPrimes2x[] = {
	UINT32_C(3), UINT32_C(1431655765),
	UINT32_C(7), UINT32_C(613566756),
	UINT32_C(13), UINT32_C(330382099),
	UINT32_C(31), UINT32_C(138547332),
	UINT32_C(61), UINT32_C(70409299),
	UINT32_C(127), UINT32_C(33818640),
	UINT32_C(251), UINT32_C(17111423),
	UINT32_C(509), UINT32_C(8438049),
	UINT32_C(1021), UINT32_C(4206628),
	UINT32_C(2039), UINT32_C(2106408),
	UINT32_C(4093), UINT32_C(1049344),
	UINT32_C(8191), UINT32_C(524352),
	UINT32_C(16381), UINT32_C(262192),
	UINT32_C(32749), UINT32_C(131148),
	UINT32_C(65521), UINT32_C(65551),
	UINT32_C(131071), UINT32_C(32768),
	UINT32_C(262139), UINT32_C(16384),
	UINT32_C(524287), UINT32_C(8192),
	UINT32_C(1048573), UINT32_C(4096),
	UINT32_C(2097143), UINT32_C(2048),
	UINT32_C(4194301), UINT32_C(1024),
	UINT32_C(8388593), UINT32_C(512),
	UINT32_C(16777213), UINT32_C(256),
	UINT32_C(33554393), UINT32_C(128),
	UINT32_C(67108859), UINT32_C(64),
	UINT32_C(134217689), UINT32_C(32),
	UINT32_C(268435399), UINT32_C(16),
	UINT32_C(536870909), UINT32_C(8),
	UINT32_C(1073741789), UINT32_C(4),
	UINT32_C(2147483647), UINT32_C(2),
	UINT32_C(4294967291), UINT32_C(1),
};

#endif

}