#ifndef SYNAPTICS_TEST_RAWDATA_H
#define SYNAPTICS_TEST_RAWDATA_H

#define TX_NUM_TRULY  (13)
#define RX_NUM_TRULY  (25)
#define DiagonalUpperLimit_TRULY  (1100)
#define DiagonalLowerLimit_TRULY  (900)

const int16_t raw_cap_data_truly_3035[TX_NUM_TRULY][RX_NUM_TRULY*2] =
{
	{1444,2407,1374,2291,1428,2381,1368,2281,1364,2273,1379,2298,1332,2221,1342,2236,1357,2261,1326,2210,1321,2201,1363,2271,1111,1852,1107,1846,1094,1823,1078,1797,1075,1791,1076,1792,1059,1765,1060,1766,1071,1785,1070,1783,1073,1788,1037,1729,1024,1707,},
	{1370,2283,1338,2230,1339,2232,1324,2207,1304,2173,1322,2204,1306,2177,1300,2167,1306,2176,1273,2122,1277,2128,1263,2106,1194,1990,1184,1973,1203,2005,1159,1932,1165,1941,1158,1930,1150,1916,1148,1914,1157,1929,1151,1919,1156,1927,1127,1878,1117,1862,},
	{1366,2277,1329,2214,1346,2243,1318,2196,1303,2172,1309,2181,1294,2156,1282,2136,1292,2153,1262,2103,1263,2105,1249,2082,1201,2002,1195,1992,1192,1986,1171,1952,1175,1959,1162,1937,1148,1914,1155,1925,1169,1948,1168,1946,1166,1943,1134,1890,1128,1879,},
	{1366,2276,1331,2218,1339,2232,1333,2222,1299,2165,1296,2161,1285,2141,1268,2113,1283,2138,1261,2102,1257,2094,1247,2078,1208,2013,1202,2004,1221,2036,1178,1963,1176,1960,1163,1938,1167,1945,1192,1986,1152,1921,1141,1901,1147,1911,1133,1889,1124,1873,},
	{1367,2278,1331,2218,1339,2231,1325,2209,1291,2152,1292,2154,1273,2122,1268,2114,1281,2135,1251,2085,1253,2089,1244,2074,1210,2017,1208,2013,1236,2059,1191,1985,1184,1973,1175,1959,1172,1954,1188,1980,1160,1933,1146,1910,1156,1927,1135,1892,1128,1881,},
	{1355,2258,1334,2223,1338,2230,1309,2182,1280,2134,1285,2142,1267,2112,1264,2108,1266,2109,1240,2066,1248,2081,1232,2052,1211,2017,1212,2020,1244,2073,1197,1995,1193,1988,1184,1973,1190,1984,1215,2025,1163,1938,1151,1918,1156,1926,1136,1892,1135,1891,},
	{1330,2217,1317,2194,1311,2185,1303,2171,1287,2145,1287,2146,1263,2106,1259,2099,1263,2105,1238,2064,1243,2071,1225,2040,1209,2015,1232,2053,1214,2023,1214,2023,1219,2030,1228,2046,1170,1950,1179,1966,1172,1954,1159,1932,1159,1932,1138,1896,1148,1913,},
	{1318,2197,1316,2194,1306,2177,1300,2166,1286,2143,1294,2157,1261,2102,1256,2094,1266,2110,1227,2045,1228,2046,1219,2032,1211,2019,1250,2083,1217,2028,1214,2023,1206,2010,1197,1995,1176,1960,1186,1976,1180,1966,1165,1941,1169,1948,1156,1927,1157,1928,},
	{1307,2178,1297,2162,1299,2164,1292,2153,1279,2131,1281,2135,1260,2100,1252,2086,1251,2084,1218,2030,1221,2033,1212,2019,1213,2022,1220,2033,1218,2030,1198,1996,1207,2010,1198,1997,1183,1972,1190,1984,1187,1978,1170,1950,1179,1966,1177,1962,1165,1942,},
	{1302,2170,1299,2165,1304,2173,1290,2150,1282,2137,1277,2128,1258,2097,1255,2092,1257,2095,1223,2038,1225,2042,1213,2022,1228,2046,1268,2113,1229,2049,1227,2045,1223,2038,1209,2015,1196,1994,1199,1999,1197,1995,1182,1969,1193,1989,1192,1987,1173,1954,},
	{1296,2160,1287,2145,1298,2163,1291,2152,1275,2125,1273,2121,1259,2099,1253,2088,1249,2081,1224,2040,1225,2042,1213,2020,1255,2091,1255,2091,1243,2071,1229,2048,1236,2059,1221,2036,1211,2019,1213,2022,1205,2007,1199,1998,1210,2016,1200,2000,1177,1961,},
	{1293,2156,1282,2136,1296,2161,1299,2165,1267,2112,1270,2116,1250,2083,1246,2077,1240,2067,1218,2029,1217,2028,1206,2009,1298,2163,1334,2224,1261,2101,1247,2079,1259,2099,1244,2074,1239,2066,1238,2063,1224,2040,1220,2033,1231,2052,1222,2036,1214,2023,},
	{1254,2090,1237,2061,1241,2068,1224,2040,1207,2012,1203,2006,1185,1975,1179,1964,1170,1950,1146,1910,1143,1905,1129,1882,1611,2684,1434,2391,1378,2297,1336,2227,1334,2223,1315,2192,1304,2174,1297,2162,1286,2143,1277,2128,1288,2146,1286,2144,1326,2210,},
};


#define TX_NUM_WINTEK  (13)
#define RX_NUM_WINTEK  (24)
#define DiagonalUpperLimit_WINTEK  (1100)
#define DiagonalLowerLimit_WINTEK  (900)

const int16_t raw_cap_data_wintek_9093[TX_NUM_WINTEK][RX_NUM_WINTEK*2] =
{
	{1512,2519,1388,2314,1389,2316,1332,2220,1313,2188,1295,2159,1275,2125,1257,2094,1250,2083,1232,2053,1238,2064,1273,2122, 986,1644, 986,1643, 977,1628, 964,1607, 948,1580, 937,1562, 925,1542, 945,1575, 932,1553, 919,1532, 899,1498, 870,1450,},
	{1398,2330,1335,2224,1307,2178,1286,2144,1263,2105,1244,2073,1218,2030,1196,1993,1188,1979,1167,1945,1160,1933,1142,1903,1032,1720,1036,1726,1027,1712,1038,1731, 997,1661, 998,1663, 979,1632, 994,1656, 982,1636, 973,1622, 955,1592, 948,1580,},
	{1369,2281,1321,2202,1290,2150,1277,2128,1251,2086,1238,2064,1212,2020,1192,1986,1177,1961,1151,1918,1145,1908,1125,1874,1041,1736,1041,1735,1033,1722,1023,1704,1006,1677, 996,1660, 985,1642,1005,1675, 998,1663, 982,1637, 966,1611, 951,1586,},
	{1339,2231,1308,2180,1280,2133,1262,2104,1241,2069,1224,2040,1205,2008,1182,1969,1169,1949,1147,1911,1136,1893,1117,1861,1050,1749,1049,1748,1041,1735,1046,1744,1021,1701,1025,1708,1018,1696,1003,1671, 985,1642, 983,1638, 975,1625, 939,1566,},
	{1327,2212,1298,2163,1272,2121,1257,2094,1234,2056,1215,2025,1196,1994,1176,1959,1160,1933,1138,1896,1131,1886,1107,1844,1074,1790,1079,1798,1071,1785,1035,1725,1057,1762,1017,1694,1005,1675,1005,1674, 993,1655, 988,1646, 987,1644, 965,1608,},
	{1324,2207,1299,2165,1268,2113,1252,2087,1229,2048,1216,2026,1189,1982,1177,1961,1163,1939,1133,1889,1126,1877,1100,1833,1094,1823,1066,1777,1082,1803,1044,1740,1032,1720,1025,1709,1015,1692,1013,1688,1002,1670, 997,1662,1001,1668, 982,1637,},
	{1319,2198,1293,2156,1262,2104,1253,2088,1231,2052,1217,2028,1192,1986,1175,1958,1159,1931,1133,1889,1127,1878,1101,1834,1078,1797,1078,1797,1071,1784,1077,1795,1045,1742,1058,1764,1075,1792,1028,1714,1018,1696,1012,1686,1017,1694, 986,1643,},
	{1313,2188,1293,2156,1263,2105,1254,2090,1230,2050,1218,2030,1190,1983,1169,1949,1156,1926,1136,1893,1127,1878,1096,1827,1095,1826,1092,1821,1085,1808,1072,1786,1058,1764,1053,1755,1042,1736,1040,1733,1028,1713,1023,1705,1031,1718, 988,1646,},
	{1315,2192,1292,2153,1273,2121,1259,2098,1236,2060,1214,2023,1192,1987,1175,1958,1159,1931,1133,1888,1121,1869,1090,1817,1146,1910,1108,1847,1113,1855,1089,1815,1078,1797,1071,1784,1059,1765,1059,1766,1047,1745,1041,1735,1049,1749,1022,1703,},
	{1312,2186,1298,2163,1267,2112,1257,2094,1232,2053,1217,2029,1194,1990,1177,1962,1159,1932,1135,1892,1131,1885,1098,1829,1143,1906,1129,1882,1121,1868,1105,1842,1094,1824,1088,1814,1076,1793,1075,1792,1066,1777,1058,1764,1068,1780,1041,1735,},
	{1308,2181,1292,2153,1266,2110,1247,2078,1223,2039,1209,2015,1189,1982,1170,1949,1155,1925,1128,1880,1120,1867,1090,1817,1161,1935,1156,1927,1141,1901,1121,1868,1145,1908,1102,1836,1091,1818,1091,1819,1080,1800,1072,1787,1081,1801,1059,1765,},
	{1311,2185,1284,2140,1263,2105,1250,2083,1224,2040,1205,2009,1182,1971,1165,1942,1151,1918,1121,1869,1118,1863,1084,1807,1193,1988,1174,1956,1158,1931,1140,1900,1127,1879,1117,1862,1110,1849,1109,1849,1098,1830,1094,1823,1104,1840,1103,1838,},
	{1269,2115,1237,2062,1208,2014,1194,1989,1171,1951,1147,1911,1124,1873,1105,1841,1083,1804,1058,1763,1045,1742,1017,1695,1632,2720,1363,2272,1284,2139,1243,2072,1215,2025,1197,1995,1184,1974,1180,1967,1166,1944,1163,1938,1179,1965,1217,2028,},
};


#define TX_NUM_TPK  (13)
#define RX_NUM_TPK  (24)
#define DiagonalUpperLimit_TPK  (1100)
#define DiagonalLowerLimit_TPK  (900)

const int16_t raw_cap_data_tpk[TX_NUM_TPK][RX_NUM_TPK*2] =
{
	{1809, 3015, 1812, 3020, 1754, 2923, 1743, 2904, 1731, 2885, 1722, 2869, 1711, 2852, 1701, 2835, 1692, 2821, 1688, 2814, 1696, 2827, 1794, 2989, 1455, 2424, 1449, 2414, 1440, 2399, 1429, 2383, 1422, 2370, 1410, 2351, 1428, 2381, 1417, 2362, 1406, 2343, 1376, 2294, 1392, 2319, 1105, 1842},
	{1759, 2932, 1735, 2891, 1710, 2850, 1695, 2825, 1681, 2803, 1671, 2786, 1658, 2763, 1644, 2740, 1633, 2722, 1624, 2706, 1614, 2690, 1621, 2702, 1532, 2553, 1526, 2543, 1518, 2530, 1510, 2516, 1512, 2521, 1519, 2532, 1509, 2516, 1498, 2497, 1484, 2474, 1458, 2431, 1476, 2459, 1213, 2021},
	{1725, 2875, 1712, 2854, 1689, 2816, 1677, 2796, 1663, 2772, 1654, 2756, 1640, 2733, 1625, 2709, 1616, 2694, 1606, 2677, 1596, 2660, 1595, 2658, 1533, 2554, 1528, 2546, 1519, 2531, 1511, 2518, 1504, 2507, 1496, 2494, 1494, 2490, 1526, 2543, 1493, 2488, 1466, 2444, 1482, 2470, 1187, 1978},
	{1703, 2838, 1690, 2817, 1674, 2790, 1662, 2770, 1649, 2748, 1641, 2734, 1628, 2714, 1615, 2691, 1604, 2673, 1594, 2657, 1583, 2638, 1580, 2633, 1533, 2554, 1528, 2547, 1521, 2535, 1517, 2529, 1526, 2543, 1514, 2523, 1519, 2532, 1506, 2510, 1485, 2474, 1468, 2447, 1473, 2455, 1170, 1950},
	{1694, 2823, 1679, 2799, 1665, 2775, 1652, 2753, 1640, 2734, 1632, 2720, 1621, 2701, 1608, 2679, 1597, 2662, 1584, 2641, 1573, 2622, 1569, 2616, 1549, 2582, 1550, 2583, 1541, 2569, 1549, 2581, 1507, 2512, 1497, 2495, 1494, 2491, 1490, 2483, 1478, 2464, 1469, 2448, 1468, 2446, 1199, 1998},
	{1693, 2822, 1676, 2793, 1661, 2769, 1649, 2749, 1636, 2726, 1626, 2710, 1614, 2690, 1602, 2671, 1592, 2654, 1578, 2630, 1566, 2610, 1562, 2603, 1566, 2611, 1558, 2597, 1530, 2549, 1518, 2531, 1513, 2521, 1503, 2504, 1500, 2499, 1494, 2490, 1484, 2474, 1475, 2458, 1475, 2458, 1205, 2008},
	{1689, 2814, 1669, 2782, 1656, 2760, 1643, 2738, 1629, 2715, 1619, 2698, 1608, 2680, 1595, 2659, 1585, 2642, 1572, 2620, 1559, 2599, 1555, 2591, 1544, 2574, 1540, 2567, 1534, 2556, 1530, 2550, 1549, 2581, 1536, 2560, 1558, 2597, 1501, 2501, 1491, 2485, 1481, 2468, 1481, 2468, 1180, 1967},
	{1687, 2812, 1669, 2781, 1652, 2753, 1640, 2734, 1626, 2710, 1619, 2698, 1607, 2678, 1593, 2654, 1583, 2637, 1570, 2617, 1559, 2598, 1554, 2591, 1556, 2593, 1553, 2588, 1552, 2586, 1572, 2620, 1532, 2554, 1525, 2541, 1523, 2538, 1514, 2523, 1503, 2505, 1494, 2490, 1493, 2488, 1185, 1976},
	{1676, 2793, 1666, 2777, 1651, 2751, 1639, 2732, 1626, 2710, 1616, 2694, 1604, 2673, 1590, 2649, 1579, 2631, 1565, 2608, 1553, 2588, 1548, 2580, 1564, 2606, 1558, 2597, 1552, 2586, 1545, 2574, 1540, 2567, 1531, 2552, 1529, 2548, 1523, 2538, 1514, 2523, 1503, 2505, 1504, 2507, 1218, 2031},
	{1673, 2788, 1663, 2772, 1646, 2744, 1637, 2729, 1625, 2709, 1616, 2694, 1605, 2675, 1591, 2651, 1580, 2633, 1567, 2611, 1555, 2592, 1550, 2583, 1629, 2716, 1596, 2660, 1577, 2628, 1565, 2608, 1558, 2597, 1549, 2582, 1546, 2576, 1538, 2564, 1529, 2548, 1519, 2532, 1518, 2529, 1226, 2043},
	{1674, 2790, 1664, 2773, 1647, 2745, 1636, 2726, 1624, 2707, 1614, 2691, 1602, 2669, 1587, 2644, 1577, 2629, 1564, 2607, 1552, 2586, 1547, 2578, 1626, 2711, 1607, 2678, 1593, 2656, 1582, 2637, 1576, 2627, 1566, 2610, 1563, 2604, 1554, 2591, 1543, 2571, 1533, 2555, 1526, 2544, 1230, 2051},
	{1678, 2796, 1670, 2783, 1654, 2757, 1642, 2737, 1628, 2713, 1615, 2692, 1600, 2666, 1584, 2640, 1573, 2621, 1560, 2600, 1548, 2580, 1542, 2571, 1731, 2886, 1638, 2730, 1624, 2707, 1605, 2675, 1597, 2662, 1586, 2643, 1582, 2636, 1574, 2623, 1564, 2607, 1555, 2592, 1546, 2577, 1281, 2135},
	{1622, 2703, 1609, 2682, 1591, 2651, 1577, 2629, 1560, 2600, 1545, 2574, 1525, 2542, 1508, 2513, 1496, 2493, 1481, 2468, 1469, 2448, 1461, 2436, 2024, 3373, 1846, 3077, 1769, 2949, 1725, 2875, 1695, 2825, 1670, 2784, 1658, 2764, 1647, 2744, 1635, 2725, 1628, 2713, 1632, 2720, 1408, 2347},
};


#define TX_NUM_TPK_N1  (15)
#define RX_NUM_TPK_N1  (26)

const int16_t raw_cap_data_tpk_n1[TX_NUM_TPK_N1][RX_NUM_TPK_N1*2] =
{
    {1653,2939,1680,2986,1686,2997,1695,3014,1699,3021,1702,3027,1711,3040,1719,3057,1723,3064,1733,3080,1735,3084,1743,3100,1748,3108,2013,3579,2004,3561,2014,3580,2017,3585,2020,3591,2027,3602,2033,3614,2039,3624,2040,3627,2048,3640,2047,3639,2049,3642,1975,3510}, 
    {1720,3059,1748,3108,1754,3117,1762,3132,1765,3140,1768,3144,1775,3156,1784,3171,1788,3178,1795,3192,1798,3197,1805,3208,1809,3215,1922,3416,1927,3425,1934,3439,1942,3453,1950,3466,1958,3482,1963,3490,1971,3504,1977,3513,1983,3527,1992,3541,1997,3549,1990,3537}, 
    {1724,3065,1751,3113,1756,3122,1764,3138,1769,3145,1771,3150,1779,3162,1787,3176,1790,3183,1798,3196,1800,3199,1807,3212,1812,3219,1906,3388,1913,3402,1921,3416,1931,3431,1938,3445,1947,3461,1951,3468,1959,3483,1966,3495,1973,3507,1980,3519,1987,3532,1925,3422}, 
    {1734,3083,1762,3131,1766,3140,1774,3154,1778,3161,1781,3166,1788,3178,1796,3193,1800,3199,1807,3211,1809,3215,1815,3228,1820,3236,1901,3379,1910,3397,1919,3411,1927,3425,1934,3440,1944,3456,1995,3547,2004,3564,2001,3557,1980,3519,1981,3522,1988,3534,1921,3416}, 
    {1738,3089,1764,3138,1770,3148,1778,3161,1781,3166,1785,3172,1791,3185,1799,3198,1802,3203,1810,3216,1812,3222,1818,3233,1823,3240,1887,3356,1900,3377,1908,3390,1916,3406,1925,3422,1934,3439,1939,3448,1950,3465,1957,3480,1965,3494,1974,3509,1981,3524,1916,3407}, 
    {1749,3110,1775,3156,1781,3166,1788,3180,1792,3186,1795,3191,1801,3202,1809,3215,1812,3220,1819,3235,1821,3238,1828,3250,1832,3257,1883,3346,1895,3368,1904,3384,1913,3401,1923,3418,1932,3433,1937,3445,1948,3463,1956,3479,1966,3495,1978,3515,1995,3546,1953,3471}, 
    {1763,3132,1788,3180,1794,3190,1801,3202,1805,3209,1808,3213,1813,3224,1821,3237,1824,3242,1831,3255,1833,3258,1840,3272,1844,3279,1878,3338,1890,3361,1900,3378,1910,3396,1919,3411,1929,3428,1935,3441,1948,3463,1956,3478,1999,3554,1984,3528,1987,3532,1919,3412}, 
    {1772,3151,1798,3197,1804,3207,1812,3220,1814,3227,1817,3231,1823,3241,1831,3254,1833,3258,1840,3272,1842,3275,1849,3287,1853,3294,1869,3323,1882,3344,1891,3363,1901,3379,1910,3397,1921,3415,1929,3428,1938,3447,1949,3464,1958,3482,1971,3504,2003,3560,1939,3447}, 
    {1788,3178,1813,3224,1818,3233,1825,3245,1829,3251,1831,3255,1836,3266,1844,3279,1846,3283,1854,3295,1856,3298,1861,3310,1865,3316,1863,3313,1876,3335,1885,3353,1896,3370,1906,3388,1916,3407,1924,3419,1935,3441,1945,3457,1990,3538,1986,3532,1995,3547,1927,3425}, 
    {1803,3206,1828,3249,1833,3258,1839,3271,1842,3275,1844,3279,1850,3289,1858,3302,1860,3306,1866,3319,1868,3321,1874,3331,1878,3338,1858,3303,1871,3326,1881,3343,1890,3361,1901,3379,1910,3397,1939,3448,1978,3516,1966,3495,1955,3474,1962,3488,1971,3503,1907,3388}, 
    {1818,3232,1841,3273,1845,3281,1853,3293,1856,3298,1858,3302,1863,3313,1870,3325,1873,3329,1879,3339,1880,3342,1885,3352,1889,3358,1853,3294,1865,3316,1875,3334,1895,3368,1905,3385,1909,3395,1932,3437,1949,3465,1954,3473,1950,3465,1956,3478,1964,3493,1901,3378}, 
    {1833,3257,1855,3297,1860,3305,1865,3316,1868,3321,1870,3325,1876,3334,1882,3345,1884,3348,1889,3359,1890,3361,1896,3370,1899,3376,1848,3285,1860,3309,1871,3326,1882,3345,1892,3363,1901,3379,1932,3433,1954,3473,1949,3464,1938,3447,1949,3464,1958,3482,1895,3368}, 
    {1852,3292,1872,3329,1876,3335,1882,3345,1884,3350,1885,3353,1891,3362,1897,3372,1899,3375,1904,3384,1908,3391,1921,3415,1914,3404,1846,3282,1860,3306,1869,3322,1894,3367,1906,3387,1918,3411,1937,3445,1920,3413,1939,3448,1936,3442,1946,3460,1956,3476,1892,3363}, 
    {1877,3336,1894,3368,1897,3373,1903,3382,1906,3387,1906,3387,1910,3397,1916,3406,1917,3409,1922,3417,1927,3426,1936,3443,1932,3434,1846,3282,1860,3305,1868,3321,1895,3369,1908,3391,1923,3419,1908,3391,1914,3404,1925,3422,1933,3438,1944,3457,1954,3473,1890,3361}, 
    {1993,3543,1982,3525,1985,3530,1985,3529,1983,3526,1981,3523,1987,3533,1992,3541,1993,3542,2002,3557,2038,3623,2055,3654,2038,3623,1792,3187,1808,3214,1818,3232,1845,3281,1860,3306,1870,3325,1858,3302,1869,3322,1880,3342,1891,3362,1903,3383,1913,3402,1856,3298}, 
};


#define TX_NUM_YFO_N1  (15)
#define RX_NUM_YFO_N1  (26)
#define DiagonalUpperLimit_YFO  (1100)
#define DiagonalLowerLimit_YFO  (900)

const int16_t raw_cap_data_yfo_n1[TX_NUM_YFO_N1][RX_NUM_YFO_N1*2] =
{
    {1404,2498,1431,2545,1428,2540,1432,2547,1433,2548,1431,2545,1432,2547,1433,2548,1439,2559,1441,2562,1444,2566,1434,2550,1439,2559,1644,2924,1633,2903,1635,2907,1639,2912,1644,2924,1648,2930,1652,2937,1656,2944,1668,2964,1675,2979,1684,2993,1699,3021,1642,2917}, 
    {1447,2572,1476,2622,1473,2618,1477,2628,1481,2633,1479,2631,1480,2631,1477,2627,1483,2638,1482,2636,1484,2639,1472,2616,1476,2625,1601,2846,1603,2851,1605,2854,1613,2867,1620,2882,1620,2879,1623,2887,1629,2896,1640,2914,1645,2926,1661,2953,1669,2969,1669,2968}, 
    {1449,2575,1477,2628,1475,2620,1480,2632,1480,2633,1482,2635,1482,2635,1482,2636,1488,2646,1484,2639,1487,2643,1474,2620,1476,2626,1592,2829,1597,2840,1598,2842,1601,2846,1607,2857,1612,2866,1618,2875,1625,2890,1636,2907,1644,2922,1657,2946,1662,2953,1611,2864}, 
    {1455,2588,1483,2637,1479,2631,1486,2641,1487,2644,1489,2647,1490,2649,1490,2649,1498,2661,1493,2654,1495,2657,1481,2633,1485,2640,1591,2828,1597,2840,1598,2842,1602,2848,1610,2862,1616,2871,1668,2966,1672,2973,1673,2975,1658,2947,1659,2948,1662,2954,1607,2858}, 
    {1460,2595,1487,2643,1483,2637,1489,2648,1491,2651,1493,2654,1494,2656,1496,2658,1501,2669,1498,2662,1500,2666,1488,2645,1490,2649,1589,2824,1596,2839,1598,2842,1602,2849,1610,2863,1618,2875,1622,2884,1630,2898,1636,2908,1646,2928,1658,2948,1662,2954,1602,2848}, 
    {1468,2609,1495,2657,1490,2648,1496,2659,1498,2662,1498,2662,1500,2665,1501,2669,1508,2681,1504,2675,1508,2681,1496,2659,1500,2665,1586,2819,1596,2835,1598,2841,1603,2850,1613,2867,1621,2882,1625,2890,1634,2904,1640,2915,1651,2936,1665,2959,1675,2978,1642,2917}, 
    {1476,2622,1501,2670,1496,2659,1501,2669,1504,2674,1502,2672,1504,2675,1507,2680,1517,2696,1516,2695,1520,2701,1505,2676,1508,2681,1584,2816,1590,2827,1599,2843,1604,2851,1610,2863,1621,2882,1628,2894,1637,2910,1638,2912,1681,2989,1666,2961,1665,2959,1601,2847}, 
    {1483,2636,1506,2679,1502,2672,1508,2681,1507,2680,1507,2679,1509,2684,1514,2692,1524,2707,1521,2704,1526,2715,1511,2687,1517,2696,1586,2820,1588,2822,1600,2846,1602,2849,1607,2857,1620,2879,1624,2888,1630,2897,1636,2907,1645,2925,1655,2942,1679,2985,1620,2881}, 
    {1490,2648,1512,2689,1509,2683,1514,2692,1515,2693,1513,2690,1516,2695,1521,2703,1527,2716,1525,2713,1532,2724,1518,2697,1524,2707,1582,2812,1584,2817,1595,2834,1599,2844,1604,2851,1614,2869,1619,2877,1622,2885,1630,2898,1670,2970,1667,2962,1670,2970,1606,2856}, 
    {1500,2665,1521,2703,1516,2695,1522,2704,1522,2705,1520,2702,1523,2706,1527,2715,1533,2726,1532,2724,1539,2735,1523,2706,1526,2713,1572,2793,1582,2812,1583,2814,1591,2828,1600,2845,1608,2859,1632,2901,1662,2954,1648,2931,1635,2906,1640,2915,1645,2926,1587,2821}, 
    {1512,2687,1531,2722,1525,2713,1529,2718,1529,2719,1527,2716,1527,2716,1533,2725,1538,2734,1538,2734,1543,2742,1524,2711,1528,2717,1564,2780,1575,2801,1579,2807,1591,2828,1602,2848,1603,2850,1625,2890,1634,2905,1634,2905,1628,2895,1632,2901,1640,2915,1581,2811}, 
    {1526,2713,1545,2745,1539,2736,1540,2738,1541,2739,1539,2735,1539,2736,1542,2741,1548,2753,1546,2747,1550,2757,1533,2725,1536,2730,1561,2775,1571,2792,1574,2800,1578,2807,1590,2825,1595,2834,1622,2885,1640,2914,1630,2898,1618,2876,1623,2887,1637,2909,1581,2810}, 
    {1543,2742,1560,2774,1556,2766,1556,2767,1555,2766,1557,2768,1557,2768,1559,2771,1564,2779,1562,2777,1569,2788,1560,2773,1551,2758,1558,2770,1568,2787,1572,2793,1589,2824,1601,2848,1611,2864,1629,2897,1607,2857,1622,2884,1618,2875,1621,2883,1635,2907,1578,2806}, 
    {1563,2778,1577,2805,1573,2797,1574,2800,1573,2798,1573,2798,1578,2806,1579,2808,1584,2815,1587,2821,1590,2826,1582,2813,1578,2806,1556,2766,1567,2785,1570,2791,1591,2827,1607,2857,1618,2876,1600,2846,1602,2848,1608,2858,1613,2867,1618,2875,1631,2900,1576,2803}, 
    {1646,2927,1644,2924,1639,2913,1639,2912,1636,2908,1635,2907,1638,2912,1641,2916,1642,2919,1649,2932,1684,2993,1693,3011,1656,2945,1525,2713,1539,2736,1544,2745,1564,2781,1583,2814,1589,2825,1572,2794,1579,2808,1584,2817,1592,2829,1596,2839,1604,2852,1559,2772}, 
};

#endif


