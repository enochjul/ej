//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <float.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#include <immintrin.h>
#include <x86intrin.h>

#include "BitScan.h"
#include "Common.h"
#include "StringTo.h"
#include "duint.h"

namespace ej {

const uint8_t HexDigitValues[] = {
	0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

StringToReturnType<int32_t> c_string_to_int32(const char *s) noexcept {
	uint32_t value32;
	unsigned digit;
	unsigned char ch;

	ch = static_cast<unsigned char>(s[0]);
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value32 = digit;
		if (digit > 0) {
			ch = static_cast<unsigned char>(s[1]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[2]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[3]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[4]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[5]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[6]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[7]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[8]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[9]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				if (EJ_LIKELY(value32 < UINT32_C(214748364)) || (value32 == UINT32_C(214748364) && digit <= 7)) {
					value32 = value32 * 10 + digit;
					ch = static_cast<unsigned char>(s[10]);
					return { static_cast<int32_t>(value32), ch == '\0' };
				} else {
					return { static_cast<int32_t>(value32), false };
				}
			} else {
				return { static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
			}
		} else {
			ch = static_cast<unsigned char>(s[1]);
			if (EJ_LIKELY(ch == 'x')) {
				auto s_plus_1 = s + 1;
				do {
					ch = static_cast<unsigned char>(s[2]);
					s++;
				} while (ch == '0');

				digit = HexDigitValues[ch];
				if (EJ_LIKELY(digit <= 15)) {
					value32 = digit;

					ch = static_cast<unsigned>(s[2]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[3]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[4]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[5]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[6]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[7]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[8]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[9]);
					return { static_cast<int32_t>(value32), ch == '\0' && value32 <= UINT32_C(2147483647) };
				} else {
					return { 0, digit == 0x7f && s > s_plus_1 };
				}
			} else {
				return { 0, ch == '\0' };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - static_cast<unsigned>('0')))) {
		ch = static_cast<unsigned char>(s[1]);
		digit = static_cast<unsigned>(ch) - '0';
		if (EJ_LIKELY(digit <= 9)) {
			value32 = digit;
			if (digit > 0) {
				ch = static_cast<unsigned char>(s[2]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[3]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[4]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[5]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[6]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[7]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[8]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[9]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[10]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					if (EJ_LIKELY(value32 < UINT32_C(214748364)) || (value32 == UINT32_C(214748364) && digit <= 8)) {
						value32 = value32 * 10 + digit;
						ch = static_cast<unsigned char>(s[11]);
						return { -static_cast<int32_t>(value32), ch == '\0' };
					} else {
						return { -static_cast<int32_t>(value32), false };
					}
				} else {
					return { -static_cast<int32_t>(value32), digit == -static_cast<unsigned>('0') };
				}
			} else {
				ch = static_cast<unsigned char>(s[2]);
				if (EJ_LIKELY(ch == 'x')) {
					auto s_plus_1 = s + 1;
					do {
						ch = static_cast<unsigned char>(s[3]);
						s++;
					} while (ch == '0');

					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = digit;

						ch = static_cast<unsigned>(s[3]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[4]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[5]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[6]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[7]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[8]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[9]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int32_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[10]);
						return { -static_cast<int32_t>(value32), ch == '\0' && value32 <= UINT32_C(2147483648) };
					} else {
						return { 0, digit == 0x7f && s > s_plus_1 };
					}
				} else {
					return { 0, ch == '\0' };
				}
			}
		}
	}
	return { 0, false };
}

StringToReturnType<int64_t> c_string_to_int64(const char *s) noexcept {
	uint32_t value32;
	uint64_t value64;
	unsigned digit;
	unsigned char ch;

	ch = static_cast<unsigned char>(s[0]);
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value32 = digit;
		if (digit > 0) {
			ch = static_cast<unsigned char>(s[1]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[2]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[3]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[4]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[5]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[6]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[7]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[8]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[9]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = static_cast<uint64_t>(value32) * 10 + digit;
			} else {
				return { static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[10]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[11]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[12]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[13]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[14]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[15]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[16]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[17]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[18]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				if (EJ_LIKELY(value64 < UINT64_C(922337203685477580)) || (value64 == UINT64_C(922337203685477580) && digit <= 7)) {
					value64 = value64 * 10 + digit;
					ch = static_cast<unsigned char>(s[19]);
					return { static_cast<int64_t>(value64), ch == '\0' };
				} else {
					return { static_cast<int64_t>(value64), false };
				}
			} else {
				return { static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
			}
		} else {
			ch = static_cast<unsigned char>(s[1]);
			if (EJ_LIKELY(ch == 'x')) {
				auto s_plus_1 = s + 1;
				do {
					ch = static_cast<unsigned char>(s[2]);
					s++;
				} while (ch == '0');

				digit = HexDigitValues[ch];
				if (EJ_LIKELY(digit <= 15)) {
					value32 = digit;

					ch = static_cast<unsigned>(s[2]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[3]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[4]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[5]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[6]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[7]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[8]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[9]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = static_cast<uint64_t>(value32) * 16 + digit;
					} else {
						return { static_cast<int64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[10]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[11]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[12]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[13]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[14]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[15]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[16]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<int64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[17]);
					return { static_cast<int64_t>(value64), ch == '\0' && value64 <= UINT64_C(9223372036854775807) };
				} else {
					return { 0, digit == 0x7f && s > s_plus_1 };
				}
			} else {
				return { 0, ch == '\0' };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - static_cast<unsigned>('0')))) {
		ch = static_cast<unsigned char>(s[1]);
		digit = static_cast<unsigned>(ch) - '0';
		if (EJ_LIKELY(digit <= 9)) {
			value32 = digit;
			if (digit > 0) {
				ch = static_cast<unsigned char>(s[2]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[3]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[4]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[5]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[6]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[7]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[8]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[9]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value32 = value32 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[10]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = static_cast<uint64_t>(value32) * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value32), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[11]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[12]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[13]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[14]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[15]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[16]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[17]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[18]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					value64 = value64 * 10 + digit;
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}

				ch = static_cast<unsigned char>(s[19]);
				digit = static_cast<unsigned>(ch) - '0';
				if (EJ_LIKELY(digit <= 9)) {
					if (EJ_LIKELY(value64 < UINT64_C(922337203685477580)) || (value64 == UINT64_C(922337203685477580) && digit <= 8)) {
						value64 = value64 * 10 + digit;
						ch = static_cast<unsigned char>(s[20]);
						return { -static_cast<int64_t>(value64), ch == '\0' };
					} else {
						return { -static_cast<int64_t>(value64), false };
					}
				} else {
					return { -static_cast<int64_t>(value64), digit == -static_cast<unsigned>('0') };
				}
			} else {
				ch = static_cast<unsigned char>(s[2]);
				if (EJ_LIKELY(ch == 'x')) {
					auto s_plus_1 = s + 1;
					do {
						ch = static_cast<unsigned char>(s[3]);
						s++;
					} while (ch == '0');

					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = digit;

						ch = static_cast<unsigned>(s[3]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[4]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[5]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[6]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[7]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[8]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[9]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value32 = value32 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[10]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = static_cast<uint64_t>(value32) * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value32), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[11]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[12]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[13]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[14]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[15]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[16]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[17]);
						digit = HexDigitValues[ch];
						if (EJ_LIKELY(digit <= 15)) {
							value64 = value64 * 16 + digit;
						} else {
							return { -static_cast<int64_t>(value64), digit == 0x7f };
						}

						ch = static_cast<unsigned>(s[18]);
						return { -static_cast<int64_t>(value64), ch == '\0' && value64 <= UINT64_C(9223372036854775808) };
					} else {
						return { 0, digit == 0x7f && s > s_plus_1 };
					}
				} else {
					return { 0, ch == '\0' };
				}
			}
		}
	}
	return { 0, false };
}

StringToReturnType<uint32_t> c_string_to_uint32(const char *s) noexcept {
	uint32_t value32;
	unsigned digit;
	unsigned char ch;

	ch = static_cast<unsigned char>(s[0]);
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value32 = digit;
		if (digit > 0) {
			ch = static_cast<unsigned char>(s[1]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[2]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[3]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[4]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[5]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[6]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[7]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[8]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[9]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				if (EJ_LIKELY(value32 < UINT32_C(429496729)) || (value32 == UINT32_C(429496729) && digit <= 5)) {
					value32 = value32 * 10 + digit;
					ch = static_cast<unsigned char>(s[10]);
					return { static_cast<uint32_t>(value32), ch == '\0' };
				} else {
					return { static_cast<uint32_t>(value32), false };
				}
			} else {
				return { static_cast<uint32_t>(value32), digit == -static_cast<unsigned>('0') };
			}
		} else {
			ch = static_cast<unsigned char>(s[1]);
			if (EJ_LIKELY(ch == 'x')) {
				auto s_plus_1 = s + 1;
				do {
					ch = static_cast<unsigned char>(s[2]);
					s++;
				} while (ch == '0');

				digit = HexDigitValues[ch];
				if (EJ_LIKELY(digit <= 15)) {
					value32 = digit;

					ch = static_cast<unsigned>(s[2]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[3]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[4]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[5]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[6]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[7]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[8]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint32_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[9]);
					return { static_cast<uint32_t>(value32), ch == '\0' };
				} else {
					return { 0, digit == 0x7f && s > s_plus_1 };
				}
			} else {
				return { 0, ch == '\0' };
			}
		}
	}
	return { 0, false };
}

StringToReturnType<uint64_t> c_string_to_uint64(const char *s) noexcept {
	uint32_t value32;
	uint64_t value64;
	unsigned digit;
	unsigned char ch;

	ch = static_cast<unsigned char>(s[0]);
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value32 = digit;
		if (digit > 0) {
			ch = static_cast<unsigned char>(s[1]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[2]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[3]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[4]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[5]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[6]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[7]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[8]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value32 = value32 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[9]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = static_cast<uint64_t>(value32) * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value32), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[10]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[11]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[12]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[13]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[14]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[15]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[16]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[17]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[18]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				value64 = value64 * 10 + digit;
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}

			ch = static_cast<unsigned char>(s[19]);
			digit = static_cast<unsigned>(ch) - '0';
			if (EJ_LIKELY(digit <= 9)) {
				if (EJ_LIKELY(value64 < UINT64_C(1844674407370955161)) || (value64 == UINT64_C(1844674407370955161) && digit <= 5)) {
					value64 = value64 * 10 + digit;
					ch = static_cast<unsigned char>(s[20]);
					return { static_cast<uint64_t>(value64), ch == '\0' };
				} else {
					return { static_cast<uint64_t>(value64), false };
				}
			} else {
				return { static_cast<uint64_t>(value64), digit == -static_cast<unsigned>('0') };
			}
		} else {
			ch = static_cast<unsigned char>(s[1]);
			if (EJ_LIKELY(ch == 'x')) {
				auto s_plus_1 = s + 1;
				do {
					ch = static_cast<unsigned char>(s[2]);
					s++;
				} while (ch == '0');

				digit = HexDigitValues[ch];
				if (EJ_LIKELY(digit <= 15)) {
					value32 = digit;

					ch = static_cast<unsigned>(s[2]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[3]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[4]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[5]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[6]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[7]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[8]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value32 = value32 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[9]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = static_cast<uint64_t>(value32) * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value32), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[10]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[11]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[12]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[13]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[14]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[15]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[16]);
					digit = HexDigitValues[ch];
					if (EJ_LIKELY(digit <= 15)) {
						value64 = value64 * 16 + digit;
					} else {
						return { static_cast<uint64_t>(value64), digit == 0x7f };
					}

					ch = static_cast<unsigned>(s[17]);
					return { static_cast<uint64_t>(value64), ch == '\0' };
				} else {
					return { 0, digit == 0x7f && s > s_plus_1 };
				}
			} else {
				return { 0, ch == '\0' };
			}
		}
	}
	return { 0, false };
}

static const float FloatPowersOf10[] = {
	1e0f,
	1e1f,
	1e2f,
	1e3f,
	1e4f,
	1e5f,
	1e6f,
	1e7f,
	1e8f,
	1e9f,
	1e10f,
};

const uint64_t NormalizedReciprocalsOfFiveUint64[] = {
	UINT64_C(0xcccccccccccccccc),
	UINT64_C(0xa3d70a3d70a3d70a),
	UINT64_C(0x83126e978d4fdf3b),
	UINT64_C(0xd1b71758e219652b),
	UINT64_C(0xa7c5ac471b478423),
	UINT64_C(0x8637bd05af6c69b5),
	UINT64_C(0xd6bf94d5e57a42bc),
	UINT64_C(0xabcc77118461cefc),
	UINT64_C(0x89705f4136b4a597),
	UINT64_C(0xdbe6fecebdedd5be),
	UINT64_C(0xafebff0bcb24aafe),
	UINT64_C(0x8cbccc096f5088cb),
	UINT64_C(0xe12e13424bb40e13),
	UINT64_C(0xb424dc35095cd80f),
	UINT64_C(0x901d7cf73ab0acd9),
	UINT64_C(0xe69594bec44de15b),
	UINT64_C(0xb877aa3236a4b449),
	UINT64_C(0x9392ee8e921d5d07),
	UINT64_C(0xec1e4a7db69561a5),
	UINT64_C(0xbce5086492111aea),
	UINT64_C(0x971da05074da7bee),
	UINT64_C(0xf1c90080baf72cb1),
	UINT64_C(0xc16d9a0095928a27),
	UINT64_C(0x9abe14cd44753b52),
	UINT64_C(0xf79687aed3eec551),
	UINT64_C(0xc612062576589dda),
	UINT64_C(0x9e74d1b791e07e48),
	UINT64_C(0xfd87b5f28300ca0d),
	UINT64_C(0xcad2f7f5359a3b3e),
	UINT64_C(0xa2425ff75e14fc31),
	UINT64_C(0x81ceb32c4b43fcf4),
	UINT64_C(0xcfb11ead453994ba),
	UINT64_C(0xa6274bbdd0fadd61),
	UINT64_C(0x84ec3c97da624ab4),
	UINT64_C(0xd4ad2dbfc3d07787),
	UINT64_C(0xaa242499697392d2),
	UINT64_C(0x881cea14545c7575),
	UINT64_C(0xd9c7dced53c72255),
	UINT64_C(0xae397d8aa96c1b77),
	UINT64_C(0x8b61313bbabce2c6),
	UINT64_C(0xdf01e85f912e37a3),
	UINT64_C(0xb267ed1940f1c61c),
	UINT64_C(0x8eb98a7a9a5b04e3),
	UINT64_C(0xe45c10c42a2b3b05),
	UINT64_C(0xb6b00d69bb55c8d1),
	UINT64_C(0x9226712162ab070d),
	UINT64_C(0xe9d71b689dde71af),
	UINT64_C(0xbb127c53b17ec159),
	UINT64_C(0x95a8637627989aad),
	UINT64_C(0xef73d256a5c0f77c),
	UINT64_C(0xbf8fdb78849a5f96),
	UINT64_C(0x993fe2c6d07b7fab),
	UINT64_C(0xf53304714d9265df),
	UINT64_C(0xc428d05aa4751e4c),
	UINT64_C(0x9ced737bb6c4183d),
	UINT64_C(0xfb158592be068d2e),
	UINT64_C(0xc8de047564d20a8b),
	UINT64_C(0xa0b19d2ab70e6ed6),
	UINT64_C(0x808e17555f3ebf11),
	UINT64_C(0xcdb02555653131b6),
	UINT64_C(0xa48ceaaab75a8e2b),
	UINT64_C(0x83a3eeeef9153e89),
	UINT64_C(0xd29fe4b18e88640e),
	UINT64_C(0xa87fea27a539e9a5),
	UINT64_C(0x86ccbb52ea94baea),
	UINT64_C(0xd7adf884aa879177),
	UINT64_C(0xac8b2d36eed2dac5),
	UINT64_C(0x8a08f0f8bf0f156b),
	UINT64_C(0xdcdb1b2798182244),
	UINT64_C(0xb0af48ec79ace837),
	UINT64_C(0x8d590723948a535f),
	UINT64_C(0xe2280b6c20dd5232),
	UINT64_C(0xb4ecd5f01a4aa828),
	UINT64_C(0x90bd77f3483bb9b9),
	UINT64_C(0xe7958cb87392c2c2),
	UINT64_C(0xb94470938fa89bce),
	UINT64_C(0x9436c0760c86e30b),
	UINT64_C(0xed246723473e3813),
	UINT64_C(0xbdb6b8e905cb600f),
	UINT64_C(0x97c560ba6b0919a5),
	UINT64_C(0xf2d56790ab41c2a2),
	UINT64_C(0xc24452da229b021b),
	UINT64_C(0x9b69dbe1b548ce7c),
	UINT64_C(0xf8a95fcf88747d94),
	UINT64_C(0xc6ede63fa05d3143),
	UINT64_C(0x9f24b832e6b0f436),
	UINT64_C(0xfea126b7d78186bc),
	UINT64_C(0xcbb41ef979346bca),
	UINT64_C(0xa2f67f2dfa90563b),
	UINT64_C(0x825ecc24c873782f),
	UINT64_C(0xd097ad07a71f26b2),
	UINT64_C(0xa6dfbd9fb8e5b88e),
	UINT64_C(0x857fcae62d8493a5),
	UINT64_C(0xd59944a37c0752a2),
	UINT64_C(0xaae103b5fcd2a881),
	UINT64_C(0x88b402f7fd75539b),
	UINT64_C(0xdab99e59958885c4),
	UINT64_C(0xaefae51477a06b03),
	UINT64_C(0x8bfbea76c619ef36),
	UINT64_C(0xdff9772470297ebd),
	UINT64_C(0xb32df8e9f3546564),
	UINT64_C(0x8f57fa54c2a9eab6),
	UINT64_C(0xe55990879ddcaabd),
	UINT64_C(0xb77ada0617e3bbcb),
	UINT64_C(0x92c8ae6b464fc96f),
	UINT64_C(0xeadab0aba3b2dbe5),
	UINT64_C(0xbbe226efb628afea),
	UINT64_C(0x964e858c91ba2655),
	UINT64_C(0xf07da27a82c37088),
	UINT64_C(0xc06481fb9bcf8d39),
	UINT64_C(0x99ea0196163fa42e),
	UINT64_C(0xf64335bcf065d37d),
	UINT64_C(0xc5029163f384a931),
	UINT64_C(0x9d9ba7832936edc0),
	UINT64_C(0xfc2c3f3841f17c67),
	UINT64_C(0xc9bcff6034c13052),
	UINT64_C(0xa163ff802a3426a8),
	UINT64_C(0x811ccc668829b887),
	UINT64_C(0xce947a3da6a9273e),
	UINT64_C(0xa54394fe1eedb8fe),
	UINT64_C(0x843610cb4bf160cb),
	UINT64_C(0xd389b47879823479),
	UINT64_C(0xa93af6c6c79b5d2d),
	UINT64_C(0x87625f056c7c4a8b),
	UINT64_C(0xd89d64d57a607744),
	UINT64_C(0xad4ab7112eb3929d),
	UINT64_C(0x8aa22c0dbef60ee4),
	UINT64_C(0xddd0467c64bce4a0),
	UINT64_C(0xb1736b96b6fd83b3),
	UINT64_C(0x8df5efabc5979c8f),
	UINT64_C(0xe3231912d5bf60e6),
	UINT64_C(0xb5b5ada8aaff80b8),
	UINT64_C(0x915e2486ef32cd60),
	UINT64_C(0xe896a0d7e51e1566),
	UINT64_C(0xba121a4650e4ddeb),
	UINT64_C(0x94db483840b717ef),
	UINT64_C(0xee2ba6c0678b597f),
	UINT64_C(0xbe89523386091465),
	UINT64_C(0x986ddb5c6b3a76b7),
	UINT64_C(0xf3e2f893dec3f126),
	UINT64_C(0xc31bfa0fe5698db8),
	UINT64_C(0x9c1661a651213e2d),
	UINT64_C(0xf9bd690a1b68637b),
	UINT64_C(0xc7caba6e7c5382c8),
	UINT64_C(0x9fd561f1fd0f9bd3),
	UINT64_C(0xffbbcfe994e5c61f),
	UINT64_C(0xcc963fee10b7d1b3),
	UINT64_C(0xa3ab66580d5fdaf5),
	UINT64_C(0x82ef85133de648c4),
	UINT64_C(0xd17f3b51fca3a7a0),
	UINT64_C(0xa798fc4196e952e7),
	UINT64_C(0x8613fd0145877585),
	UINT64_C(0xd686619ba27255a2),
	UINT64_C(0xab9eb47c81f5114f),
	UINT64_C(0x894bc396ce5da772),
	UINT64_C(0xdbac6c247d62a583),
	UINT64_C(0xafbd2350644eeacf),
	UINT64_C(0x8c974f7383725573),
	UINT64_C(0xe0f218b8d25088b8),
	UINT64_C(0xb3f4e093db73a093),
	UINT64_C(0x8ff71a0fe2c2e6dc),
	UINT64_C(0xe65829b3046b0afa),
	UINT64_C(0xb84687c269ef3bfb),
	UINT64_C(0x936b9fcebb25c995),
	UINT64_C(0xebdf661791d60f56),
	UINT64_C(0xbcb2b812db11a5de),
	UINT64_C(0x96f5600f15a7b7e5),
	UINT64_C(0xf18899b1bc3f8ca1),
	UINT64_C(0xc13a148e3032d6e7),
	UINT64_C(0x9a94dd3e8cf578b9),
	UINT64_C(0xf7549530e188c128),
	UINT64_C(0xc5dd44271ad3cdba),
	UINT64_C(0x9e4a9cec15763e2e),
	UINT64_C(0xfd442e4688bd304a),
	UINT64_C(0xca9cf1d206fdc03b),
	UINT64_C(0xa21727db38cb002f),
	UINT64_C(0x81ac1fe293d599bf),
	UINT64_C(0xcf79cc9db955c2cc),
	UINT64_C(0xa5fb0a17c777cf09),
	UINT64_C(0x84c8d4dfd2c63f3b),
	UINT64_C(0xd47487cc8470652b),
	UINT64_C(0xa9f6d30a038d1dbc),
	UINT64_C(0x87f8a8d4cfa417c9),
	UINT64_C(0xd98ddaee19068c76),
	UINT64_C(0xae0b158b4738705e),
	UINT64_C(0x8b3c113c38f9f37e),
	UINT64_C(0xdec681f9f4c31f31),
	UINT64_C(0xb23867fb2a35b28d),
	UINT64_C(0x8e938662882af53e),
	UINT64_C(0xe41f3d6a7377eeca),
	UINT64_C(0xb67f6455292cbf08),
	UINT64_C(0x91ff83775423cc06),
	UINT64_C(0xe998d258869facd7),
	UINT64_C(0xbae0a846d2195712),
	UINT64_C(0x9580869f0e7aac0e),
	UINT64_C(0xef340a98172aace4),
	UINT64_C(0xbf5cd54678eef0b6),
	UINT64_C(0x991711052d8bf3c5),
	UINT64_C(0xf4f1b4d515acb93b),
	UINT64_C(0xc3f490aa77bd60fc),
	UINT64_C(0x9cc3a6eec6311a63),
	UINT64_C(0xfad2a4b13d1b5d6c),
	UINT64_C(0xc8a883c0fdaf7df0),
	UINT64_C(0xa086cfcd97bf97f3),
	UINT64_C(0x806bd9714632dff6),
	UINT64_C(0xcd795be870516656),
	UINT64_C(0xa46116538d0deb78),
	UINT64_C(0x8380dea93da4bc60),
	UINT64_C(0xd267caa862a12d66),
	UINT64_C(0xa8530886b54dbdeb),
	UINT64_C(0x86a8d39ef77164bc),
	UINT64_C(0xd77485cb25823ac7),
	UINT64_C(0xac5d37d5b79b6239),
	UINT64_C(0x89e42caaf9491b60),
	UINT64_C(0xdca04777f541c567),
	UINT64_C(0xb080392cc4349dec),
	UINT64_C(0x8d3360f09cf6e4bd),
	UINT64_C(0xe1ebce4dc7f16dfb),
	UINT64_C(0xb4bca50b065abe63),
	UINT64_C(0x9096ea6f3848984f),
	UINT64_C(0xe757dd7ec07426e5),
	UINT64_C(0xb913179899f68584),
	UINT64_C(0x940f4613ae5ed136),
	UINT64_C(0xece53cec4a314ebd),
	UINT64_C(0xbd8430bd08277231),
	UINT64_C(0x979cf3ca6cec5b5a),
	UINT64_C(0xf294b943e17a2bc4),
	UINT64_C(0xc21094364dfb5636),
	UINT64_C(0x9b407691d7fc44f8),
	UINT64_C(0xf867241c8cc6d4c0),
	UINT64_C(0xc6b8e9b0709f109a),
	UINT64_C(0x9efa548d26e5a6e1),
	UINT64_C(0xfe5d54150b090b02),
	UINT64_C(0xcb7ddcdda26da268),
	UINT64_C(0xa2cb1717b52481ed),
	UINT64_C(0x823c12795db6ce57),
	UINT64_C(0xd0601d8efc57b08b),
	UINT64_C(0xa6b34ad8c9dfc06f),
	UINT64_C(0x855c3be0a17fcd26),
	UINT64_C(0xd5605fcdcf32e1d6),
	UINT64_C(0xaab37fd7d8f58178),
	UINT64_C(0x888f99797a5e012d),
	UINT64_C(0xda7f5bf590966848),
	UINT64_C(0xaecc49914078536d),
	UINT64_C(0x8bd6a141006042bd),
	UINT64_C(0xdfbdcece67006ac9),
	UINT64_C(0xb2fe3f0b8599ef07),
	UINT64_C(0x8f31cc0937ae58d2),
	UINT64_C(0xe51c79a85916f484),
	UINT64_C(0xb749faed14125d36),
	UINT64_C(0x92a1958a7675175f),
	UINT64_C(0xea9c227723ee8bcb),
	UINT64_C(0xbbb01b9283253ca2),
	UINT64_C(0x96267c7535b763b5),
	UINT64_C(0xf03d93eebc589f88),
	UINT64_C(0xc0314325637a1939),
	UINT64_C(0x99c102844f94e0fb),
	UINT64_C(0xf6019da07f549b2b),
	UINT64_C(0xc4ce17b399107c22),
	UINT64_C(0x9d71ac8fada6c9b5),
	UINT64_C(0xfbe9141915d7a922),
	UINT64_C(0xc987434744ac874e),
	UINT64_C(0xa139029f6a239f72),
	UINT64_C(0x80fa687f881c7f8e),
	UINT64_C(0xce5d73ff402d98e3),
	UINT64_C(0xa5178fff668ae0b6),
	UINT64_C(0x8412d9991ed58091),
	UINT64_C(0xd3515c2831559a83),
	UINT64_C(0xa90de3535aaae202),
	UINT64_C(0x873e4f75e2224e68),
	UINT64_C(0xd863b256369d4a40),
	UINT64_C(0xad1c8eab5ee43b66),
	UINT64_C(0x8a7d3eef7f1cfc52),
	UINT64_C(0xdd95317f31c7fa1d),
	UINT64_C(0xb1442798f49ffb4a),
	UINT64_C(0x8dd01fad907ffc3b),
	UINT64_C(0xe2e69915b3fff9f9),
	UINT64_C(0xb58547448ffffb2d),
	UINT64_C(0x91376c36d99995be),
	UINT64_C(0xe858ad248f5c22c9),
	UINT64_C(0xb9e08a83a5e34f07),
	UINT64_C(0x94b3a202eb1c3f39),
	UINT64_C(0xedec366b11c6cb8f),
	UINT64_C(0xbe5691ef416bd60c),
	UINT64_C(0x9845418c345644d6),
	UINT64_C(0xf3a20279ed56d48a),
	UINT64_C(0xc2e801fb244576d5),
	UINT64_C(0x9becce62836ac577),
	UINT64_C(0xf97ae3d0d2446f25),
	UINT64_C(0xc795830d75038c1d),
	UINT64_C(0x9faacf3df73609b1),
	UINT64_C(0xff77b1fcbebcdc4f),
	UINT64_C(0xcc5fc196fefd7d0c),
	UINT64_C(0xa37fce126597973c),
	UINT64_C(0x82cca4db847945ca),
	UINT64_C(0xd1476e2c07286faa),
	UINT64_C(0xa76c582338ed2621),
	UINT64_C(0x85f0468293f0eb4e),
	UINT64_C(0xd64d3d9db981787d),
	UINT64_C(0xab70fe17c79ac6ca),
	UINT64_C(0x892731ac9faf056e),
	UINT64_C(0xdb71e91432b1a24a),
	UINT64_C(0xaf8e5410288e1b6f),
	UINT64_C(0x8c71dcd9ba0b4925),
	UINT64_C(0xe0b62e2929aba83c),
	UINT64_C(0xb3c4f1ba87bc8696),
	UINT64_C(0x8fd0c16206306bab),
	UINT64_C(0xe61acf033d1a45df),
	UINT64_C(0xb8157268fdae9e4c),
	UINT64_C(0x93445b8731587ea3),
	UINT64_C(0xeba09271e88d976b),
	UINT64_C(0xbc807527ed3e12bc),
	UINT64_C(0x96cd2a865764dbca),
	UINT64_C(0xf148440a256e2c76),
	UINT64_C(0xc1069cd4eabe89f8),
	UINT64_C(0x9a6bb0aa55653b2d),
	UINT64_C(0xf712b443bbd52b7b),
	UINT64_C(0xc5a890362fddbc62),
	UINT64_C(0x9e20735e8cb16382),
	UINT64_C(0xfd00b897478238d0),
	UINT64_C(0xca66fa129f9b60a6),
	UINT64_C(0xa1ebfb4219491a1f),
	UINT64_C(0x818995ce7aa0e1b2),
	UINT64_C(0xcf42894a5dce35ea),
	UINT64_C(0xa5ced43b7e3e9188),
	UINT64_C(0x84a57695fe98746d),
	UINT64_C(0xd43bf0effdc0ba48),
	UINT64_C(0xa9c98d8ccb009506),
	UINT64_C(0x87d4713d6f33aa6b),
	UINT64_C(0xd953e8624b85dd78),
	UINT64_C(0xaddcb9e83c6b1793),
	UINT64_C(0x8b16fb203055ac76),
	UINT64_C(0xde8b2b66b3bc4723),
	UINT64_C(0xb208ef855c969f4f),
	UINT64_C(0x8e6d8c6ab0787f72),
	UINT64_C(0xe3e27a444d8d98b7),
	UINT64_C(0xb64ec836a47146f9),
	UINT64_C(0x91d8a02bb6c10594),
	UINT64_C(0xe95a99df8ace6f53),
	UINT64_C(0xbaaee17fa23ebf76),
	UINT64_C(0x9558b4661b6565f8),
	UINT64_C(0xeef453d6923bd65a),
};

#define EJ_MAX_SMALL_POWER_OF_FIVE_BITS 4
#define EJ_MAX_SMALL_POWER_OF_FIVE_MASK ((1u << (EJ_MAX_SMALL_POWER_OF_FIVE_BITS)) - 1)

#define EJ_FIRST_ESTIMATED_POWER_OF_FIVE_UINT64 28

static const uint64_t NormalizedEstimatedPowersOfFiveUint64[] = {
	//Powers of 5 that are represented exactly, up to 5^27
	UINT64_C(1),
	UINT64_C(5),
	UINT64_C(25),
	UINT64_C(125),
	UINT64_C(625),
	UINT64_C(3125),
	UINT64_C(15625),
	UINT64_C(78125),
	UINT64_C(390625),
	UINT64_C(1953125),
	UINT64_C(9765625),
	UINT64_C(48828125),
	UINT64_C(244140625),
	UINT64_C(1220703125),
	UINT64_C(6103515625),
	UINT64_C(30517578125),
	UINT64_C(152587890625),
	UINT64_C(762939453125),
	UINT64_C(3814697265625),
	UINT64_C(19073486328125),
	UINT64_C(95367431640625),
	UINT64_C(476837158203125),
	UINT64_C(2384185791015625),
	UINT64_C(11920928955078125),
	UINT64_C(59604644775390625),
	UINT64_C(298023223876953125),
	UINT64_C(1490116119384765625),
	UINT64_C(7450580596923828125),
	//Powers of 5 that are estimated using the first 64 bits, from 5^28 to 5^308
	UINT64_C(9313225746154785156),
	UINT64_C(11641532182693481445),
	UINT64_C(14551915228366851806),
	UINT64_C(18189894035458564758),
	UINT64_C(11368683772161602973),
	UINT64_C(14210854715202003717),
	UINT64_C(17763568394002504646),
	UINT64_C(11102230246251565404),
	UINT64_C(13877787807814456755),
	UINT64_C(17347234759768070944),
	UINT64_C(10842021724855044340),
	UINT64_C(13552527156068805425),
	UINT64_C(16940658945086006781),
	UINT64_C(10587911840678754238),
	UINT64_C(13234889800848442797),
	UINT64_C(16543612251060553497),
	UINT64_C(10339757656912845935),
	UINT64_C(12924697071141057419),
	UINT64_C(16155871338926321774),
	UINT64_C(10097419586828951109),
	UINT64_C(12621774483536188886),
	UINT64_C(15777218104420236108),
	UINT64_C(9860761315262647567),
	UINT64_C(12325951644078309459),
	UINT64_C(15407439555097886824),
	UINT64_C(9629649721936179265),
	UINT64_C(12037062152420224081),
	UINT64_C(15046327690525280101),
	UINT64_C(9403954806578300063),
	UINT64_C(11754943508222875079),
	UINT64_C(14693679385278593849),
	UINT64_C(18367099231598242312),
	UINT64_C(11479437019748901445),
	UINT64_C(14349296274686126806),
	UINT64_C(17936620343357658507),
	UINT64_C(11210387714598536567),
	UINT64_C(14012984643248170709),
	UINT64_C(17516230804060213386),
	UINT64_C(10947644252537633366),
	UINT64_C(13684555315672041708),
	UINT64_C(17105694144590052135),
	UINT64_C(10691058840368782584),
	UINT64_C(13363823550460978230),
	UINT64_C(16704779438076222788),
	UINT64_C(10440487148797639242),
	UINT64_C(13050608935997049053),
	UINT64_C(16313261169996311316),
	UINT64_C(10195788231247694572),
	UINT64_C(12744735289059618216),
	UINT64_C(15930919111324522770),
	UINT64_C(9956824444577826731),
	UINT64_C(12446030555722283414),
	UINT64_C(15557538194652854267),
	UINT64_C(9723461371658033917),
	UINT64_C(12154326714572542396),
	UINT64_C(15192908393215677995),
	UINT64_C(9495567745759798747),
	UINT64_C(11869459682199748434),
	UINT64_C(14836824602749685542),
	UINT64_C(9273015376718553464),
	UINT64_C(11591269220898191830),
	UINT64_C(14489086526122739788),
	UINT64_C(18111358157653424735),
	UINT64_C(11319598848533390459),
	UINT64_C(14149498560666738074),
	UINT64_C(17686873200833422592),
	UINT64_C(11054295750520889120),
	UINT64_C(13817869688151111400),
	UINT64_C(17272337110188889250),
	UINT64_C(10795210693868055781),
	UINT64_C(13494013367335069727),
	UINT64_C(16867516709168837158),
	UINT64_C(10542197943230523224),
	UINT64_C(13177747429038154030),
	UINT64_C(16472184286297692538),
	UINT64_C(10295115178936057836),
	UINT64_C(12868893973670072295),
	UINT64_C(16086117467087590369),
	UINT64_C(10053823416929743980),
	UINT64_C(12567279271162179975),
	UINT64_C(15709099088952724969),
	UINT64_C(9818186930595453106),
	UINT64_C(12272733663244316382),
	UINT64_C(15340917079055395478),
	UINT64_C(9588073174409622174),
	UINT64_C(11985091468012027717),
	UINT64_C(14981364335015034646),
	UINT64_C(9363352709384396654),
	UINT64_C(11704190886730495817),
	UINT64_C(14630238608413119772),
	UINT64_C(18287798260516399715),
	UINT64_C(11429873912822749822),
	UINT64_C(14287342391028437277),
	UINT64_C(17859177988785546597),
	UINT64_C(11161986242990966623),
	UINT64_C(13952482803738708279),
	UINT64_C(17440603504673385348),
	UINT64_C(10900377190420865842),
	UINT64_C(13625471488026082303),
	UINT64_C(17031839360032602879),
	UINT64_C(10644899600020376799),
	UINT64_C(13306124500025470999),
	UINT64_C(16632655625031838749),
	UINT64_C(10395409765644899218),
	UINT64_C(12994262207056124023),
	UINT64_C(16242827758820155028),
	UINT64_C(10151767349262596893),
	UINT64_C(12689709186578246116),
	UINT64_C(15862136483222807645),
	UINT64_C(9913835302014254778),
	UINT64_C(12392294127517818473),
	UINT64_C(15490367659397273091),
	UINT64_C(9681479787123295682),
	UINT64_C(12101849733904119602),
	UINT64_C(15127312167380149503),
	UINT64_C(9454570104612593439),
	UINT64_C(11818212630765741799),
	UINT64_C(14772765788457177249),
	UINT64_C(9232978617785735780),
	UINT64_C(11541223272232169725),
	UINT64_C(14426529090290212157),
	UINT64_C(18033161362862765196),
	UINT64_C(11270725851789228247),
	UINT64_C(14088407314736535309),
	UINT64_C(17610509143420669137),
	UINT64_C(11006568214637918210),
	UINT64_C(13758210268297397763),
	UINT64_C(17197762835371747204),
	UINT64_C(10748601772107342002),
	UINT64_C(13435752215134177503),
	UINT64_C(16794690268917721879),
	UINT64_C(10496681418073576174),
	UINT64_C(13120851772591970218),
	UINT64_C(16401064715739962772),
	UINT64_C(10250665447337476733),
	UINT64_C(12813331809171845916),
	UINT64_C(16016664761464807395),
	UINT64_C(10010415475915504622),
	UINT64_C(12513019344894380777),
	UINT64_C(15641274181117975972),
	UINT64_C(9775796363198734982),
	UINT64_C(12219745453998418728),
	UINT64_C(15274681817498023410),
	UINT64_C(9546676135936264631),
	UINT64_C(11933345169920330789),
	UINT64_C(14916681462400413486),
	UINT64_C(9322925914000258429),
	UINT64_C(11653657392500323036),
	UINT64_C(14567071740625403795),
	UINT64_C(18208839675781754744),
	UINT64_C(11380524797363596715),
	UINT64_C(14225655996704495894),
	UINT64_C(17782069995880619867),
	UINT64_C(11113793747425387417),
	UINT64_C(13892242184281734271),
	UINT64_C(17365302730352167839),
	UINT64_C(10853314206470104899),
	UINT64_C(13566642758087631124),
	UINT64_C(16958303447609538905),
	UINT64_C(10598939654755961816),
	UINT64_C(13248674568444952270),
	UINT64_C(16560843210556190337),
	UINT64_C(10350527006597618960),
	UINT64_C(12938158758247023701),
	UINT64_C(16172698447808779626),
	UINT64_C(10107936529880487266),
	UINT64_C(12634920662350609083),
	UINT64_C(15793650827938261354),
	UINT64_C(9871031767461413346),
	UINT64_C(12338789709326766682),
	UINT64_C(15423487136658458353),
	UINT64_C(9639679460411536470),
	UINT64_C(12049599325514420588),
	UINT64_C(15061999156893025735),
	UINT64_C(9413749473058141084),
	UINT64_C(11767186841322676356),
	UINT64_C(14708983551653345445),
	UINT64_C(18386229439566681806),
	UINT64_C(11491393399729176129),
	UINT64_C(14364241749661470161),
	UINT64_C(17955302187076837701),
	UINT64_C(11222063866923023563),
	UINT64_C(14027579833653779454),
	UINT64_C(17534474792067224318),
	UINT64_C(10959046745042015198),
	UINT64_C(13698808431302518998),
	UINT64_C(17123510539128148748),
	UINT64_C(10702194086955092967),
	UINT64_C(13377742608693866209),
	UINT64_C(16722178260867332761),
	UINT64_C(10451361413042082976),
	UINT64_C(13064201766302603720),
	UINT64_C(16330252207878254650),
	UINT64_C(10206407629923909156),
	UINT64_C(12758009537404886445),
	UINT64_C(15947511921756108056),
	UINT64_C(9967194951097567535),
	UINT64_C(12458993688871959419),
	UINT64_C(15573742111089949274),
	UINT64_C(9733588819431218296),
	UINT64_C(12166986024289022870),
	UINT64_C(15208732530361278588),
	UINT64_C(9505457831475799117),
	UINT64_C(11881822289344748896),
	UINT64_C(14852277861680936121),
	UINT64_C(9282673663550585075),
	UINT64_C(11603342079438231344),
	UINT64_C(14504177599297789180),
	UINT64_C(18130221999122236476),
	UINT64_C(11331388749451397797),
	UINT64_C(14164235936814247246),
	UINT64_C(17705294921017809058),
	UINT64_C(11065809325636130661),
	UINT64_C(13832261657045163327),
	UINT64_C(17290327071306454158),
	UINT64_C(10806454419566533849),
	UINT64_C(13508068024458167311),
	UINT64_C(16885085030572709139),
	UINT64_C(10553178144107943212),
	UINT64_C(13191472680134929015),
	UINT64_C(16489340850168661269),
	UINT64_C(10305838031355413293),
	UINT64_C(12882297539194266616),
	UINT64_C(16102871923992833270),
	UINT64_C(10064294952495520794),
	UINT64_C(12580368690619400992),
	UINT64_C(15725460863274251240),
	UINT64_C(9828413039546407025),
	UINT64_C(12285516299433008781),
	UINT64_C(15356895374291260977),
	UINT64_C(9598059608932038110),
	UINT64_C(11997574511165047638),
	UINT64_C(14996968138956309548),
	UINT64_C(9373105086847693467),
	UINT64_C(11716381358559616834),
	UINT64_C(14645476698199521043),
	UINT64_C(18306845872749401303),
	UINT64_C(11441778670468375814),
	UINT64_C(14302223338085469768),
	UINT64_C(17877779172606837210),
	UINT64_C(11173611982879273256),
	UINT64_C(13967014978599091570),
	UINT64_C(17458768723248864463),
	UINT64_C(10911730452030540289),
	UINT64_C(13639663065038175362),
	UINT64_C(17049578831297719202),
	UINT64_C(10655986769561074501),
	UINT64_C(13319983461951343127),
	UINT64_C(16649979327439178909),
	UINT64_C(10406237079649486818),
	UINT64_C(13007796349561858522),
	UINT64_C(16259745436952323153),
	UINT64_C(10162340898095201970),
	UINT64_C(12702926122619002463),
	UINT64_C(15878657653273753079),
	UINT64_C(9924161033296095674),
	UINT64_C(12405201291620119593),
	UINT64_C(15506501614525149491),
	UINT64_C(9691563509078218432),
	UINT64_C(12114454386347773040),
	UINT64_C(15143067982934716300),
	UINT64_C(9464417489334197687),
	UINT64_C(11830521861667747109),
	UINT64_C(14788152327084683887),
	UINT64_C(9242595204427927429),
	UINT64_C(11553244005534909286),
	UINT64_C(14441555006918636608),
	UINT64_C(18051943758648295760),
	UINT64_C(11282464849155184850),
	UINT64_C(14103081061443981063),
	UINT64_C(17628851326804976328),
	UINT64_C(11018032079253110205),
	UINT64_C(13772540099066387756),
	UINT64_C(17215675123832984696),
	UINT64_C(10759796952395615435),
	UINT64_C(13449746190494519293),
	UINT64_C(16812182738118149117),
	UINT64_C(10507614211323843198),
	UINT64_C(13134517764154803997),
	UINT64_C(16418147205193504997),
	UINT64_C(10261342003245940623),
};

static const uint64_t LargePowersOfFive[] = {
	UINT64_C(152587890625),	//5e16 at: 0
	UINT64_C(3273344365508751233),	//5e32 at: 1
	UINT64_C(1262),
	UINT64_C(10796982567420264257),	//5e48 at: 3
	UINT64_C(192592994438723),
	UINT64_C(7942358959831785217),	//5e64 at: 5
	UINT64_C(16807427164405733357),
	UINT64_C(1593091),
	UINT64_C(1786349750302592705),	//5e80 at: 8
	UINT64_C(17253516472054312868),
	UINT64_C(243086534291450847),
	UINT64_C(16463104408102080129),	//5e96 at: 11
	UINT64_C(8787939108875325466),
	UINT64_C(7119498668098974887),
	UINT64_C(2010764683),
	UINT64_C(3381057891661143617),	//5e112 at: 15
	UINT64_C(10735983596622975722),
	UINT64_C(8960454036513601038),
	UINT64_C(11670436401755083712),
	UINT64_C(16),
	UINT64_C(279109966635548161),	//5e128 at: 20
	UINT64_C(2554917779393558781),
	UINT64_C(14124656261812188652),
	UINT64_C(11976055582626787546),
	UINT64_C(2537941837315),
	UINT64_C(13142907561237294529),	//5e144 at: 25
	UINT64_C(15012915935620909864),
	UINT64_C(8626481361138331845),
	UINT64_C(1168207125790530789),
	UINT64_C(6693145547210207115),
	UINT64_C(20993),
	UINT64_C(5918522610932723073),	//5e160 at: 31
	UINT64_C(6274625834419431504),
	UINT64_C(15436471992936698899),
	UINT64_C(41157043676584345),
	UINT64_C(8837601482497004758),
	UINT64_C(3203332952292961),
	UINT64_C(3739996296477556033),	//5e176 at: 37
	UINT64_C(10351898152832247795),
	UINT64_C(13841419836910763695),
	UINT64_C(18311852890016993852),
	UINT64_C(16863622950885146403),
	UINT64_C(2525173035325650718),
	UINT64_C(26497349),
	UINT64_C(12241203936672963841),	//5e192 at: 44
	UINT64_C(8547193819338301733),
	UINT64_C(18349701731253264454),
	UINT64_C(8335547191915630412),
	UINT64_C(14106508276975308012),
	UINT64_C(11632508206330070277),
	UINT64_C(4043174611952194906),
	UINT64_C(4367455660591058113),	//5e208 at: 51
	UINT64_C(16548313580814331792),
	UINT64_C(4321648130621879828),
	UINT64_C(13547454815450313946),
	UINT64_C(6048736007087622621),
	UINT64_C(7420898731738094924),
	UINT64_C(13552186893076320950),
	UINT64_C(33444356521),
	UINT64_C(4777629371499269249),	//5e224 at: 59
	UINT64_C(18001405160525099782),
	UINT64_C(4879812164015670080),
	UINT64_C(14509320244552862897),
	UINT64_C(2636856509737464972),
	UINT64_C(2417179124242652079),
	UINT64_C(8851510277453438840),
	UINT64_C(11902450618118332165),
	UINT64_C(276),
	UINT64_C(12562630731880668225),	//5e240 at: 68
	UINT64_C(18148925004990573666),
	UINT64_C(13687028476692182690),
	UINT64_C(3188046834748633958),
	UINT64_C(7208039433865641912),
	UINT64_C(7642104988810794228),
	UINT64_C(4308327305980438358),
	UINT64_C(14256543960517867562),
	UINT64_C(42212712576431),
	UINT64_C(13750482914757213185),	//5e256 at: 77
	UINT64_C(1302999927698857842),
	UINT64_C(14936872543252795590),
	UINT64_C(2788415840139466767),
	UINT64_C(2095640732773017264),
	UINT64_C(7205570348933370714),
	UINT64_C(7348167152523113408),
	UINT64_C(9285516396840364274),
	UINT64_C(6907659600622710236),
	UINT64_C(349175),
	UINT64_C(5315065625431894977),	//5e272 at: 87
	UINT64_C(9314130467072152725),
	UINT64_C(10025403145359357349),
	UINT64_C(18286149486344847958),
	UINT64_C(4844151787336583626),
	UINT64_C(4545084181093574900),
	UINT64_C(12194420911872105142),
	UINT64_C(1769952130372526753),
	UINT64_C(9388647902887847758),
	UINT64_C(53279933847805372),
	UINT64_C(792478769211075457),	//5e288 at: 97
	UINT64_C(9511335878798620707),
	UINT64_C(12802697721526339408),
	UINT64_C(9401145187231568770),
	UINT64_C(9023980117402305551),
	UINT64_C(4069250660416542207),
	UINT64_C(2180080595215225784),
	UINT64_C(16801844099710515453),
	UINT64_C(10232149913123443095),
	UINT64_C(3138241419126269405),
	UINT64_C(440721283),
	UINT64_C(9460753007646669633),	//5e304 at: 108
	UINT64_C(4101240109067166068),
	UINT64_C(4318227663976943808),
	UINT64_C(13679571461471155550),
	UINT64_C(14606445316381517945),
	UINT64_C(2797963276364370362),
	UINT64_C(12008763709573679603),
	UINT64_C(3508196504893670642),
	UINT64_C(13255565951930391271),
	UINT64_C(2241350861767344549),
	UINT64_C(11908498731343941621),
	UINT64_C(3),
	UINT64_C(14401576960375167745),	//5e320 at: 120
	UINT64_C(8342440259312528684),
	UINT64_C(5880478486118623184),
	UINT64_C(11552544083348288248),
	UINT64_C(4937479739776443194),
	UINT64_C(8108476058741955002),
	UINT64_C(16551382738316049051),
	UINT64_C(13712799161325714254),
	UINT64_C(10115908636764788705),
	UINT64_C(15304990897611598971),
	UINT64_C(14763773636856087296),
	UINT64_C(556268464626),
	UINT64_C(2343036402624121537),	//5e336 at: 132
	UINT64_C(15712293936921064210),
	UINT64_C(9685200040091611846),
	UINT64_C(1824876561096011938),
	UINT64_C(3033007227748009068),
	UINT64_C(7697817146613520910),
	UINT64_C(14734893084814930886),
	UINT64_C(15135274020250680270),
	UINT64_C(16521098470741304709),
	UINT64_C(899335150196869773),
	UINT64_C(5323629405605868178),
	UINT64_C(6362155473245619239),
	UINT64_C(4601),
};

static const uint8_t LargePowersOfFiveOffsets[] = {
	0,
	1,
	3,
	5,
	8,
	11,
	15,
	20,
	25,
	31,
	37,
	44,
	51,
	59,
	68,
	77,
	87,
	97,
	108,
	120,
	132,
	145,
};

//! Rounds a 26 bit mantissa to 24 bits using round to nearest even, and updates the exponent if necessary
EJ_ALWAYS_INLINE void float_round_to_nearest_even(uint32_t *mantissa, int *exponent, uint32_t remainder) noexcept {
	uint32_t m = *mantissa;
	int e = *exponent;

	//Extract the last bit of the 24 bit mantissa and combine it with the remainder
	remainder |= m & 4;
	//Update the last bit of the mantissa based on the remainder, and use addition to implement rounding
	m |= remainder != 0 ? 1 : 0;
	++m;
	m >>= 2;
	//In the unlikely event that the mantissa is all ones, then the rounding will increase the size of the mantissa by one bit
	//Shift the mantissa back to the correct position in that case
	if ((m & (static_cast<uint32_t>(1) << FLT_MANT_DIG)) != 0) {
		m >>= 1;
		++e;
	}

	*mantissa = m;
	*exponent = e;
}

EJ_ALWAYS_INLINE void float_round_to_nearest_even(uint32_t *mantissa, int *exponent, uint64_t remainder) noexcept {
	uint32_t m = *mantissa;
	int e = *exponent;

	//Extract the last bit of the 24 bit mantissa and combine it with the remainder
	remainder |= m & 4;
	//Update the last bit of the mantissa based on the remainder, and use addition to implement rounding
	m |= remainder != 0 ? 1 : 0;
	++m;
	m >>= 2;
	//In the unlikely event that the mantissa is all ones, then the rounding will increase the size of the mantissa by one bit
	//Shift the mantissa back to the correct position in that case
	if ((m & (static_cast<uint32_t>(1) << FLT_MANT_DIG)) != 0) {
		m >>= 1;
		++e;
	}

	*mantissa = m;
	*exponent = e;
}

EJ_ALWAYS_INLINE float make_float(uint32_t mantissa, int exponent, bool negative) noexcept {
	union {
		uint32_t u;
		float f;
	} uf;

	uf.u = (static_cast<uint32_t>(negative) << (sizeof(uint32_t) * CHAR_BIT - 1)) |
		(static_cast<uint32_t>(exponent + (2 - FLT_MIN_EXP)) << (FLT_MANT_DIG - 1)) |
		(mantissa & ((static_cast<uint32_t>(1) << (FLT_MANT_DIG - 1)) - 1u));
	return uf.f;
}

EJ_ALWAYS_INLINE float make_subnormal_float(uint32_t mantissa, int exponent, uint32_t remainder, bool negative) noexcept {
	int remainder_bits;

	remainder_bits = (FLT_MIN_EXP - 1) - exponent;
	if (remainder_bits <= FLT_MANT_DIG) {
		remainder |= mantissa << (32 - remainder_bits);
		mantissa >>= remainder_bits;
		exponent = FLT_MIN_EXP - 2;

		//Round to nearest even
		remainder |= mantissa & 4;
		mantissa |= remainder != 0 ? 1 : 0;
		++mantissa;
		mantissa >>= 2;
		if ((mantissa & (static_cast<uint32_t>(1) << (FLT_MANT_DIG - 1))) != 0) {
			exponent = FLT_MIN_EXP - 1;
		}

		return make_float(mantissa, exponent, negative);
	} else {
		return !negative ? 0.f : -0.f;
	}
}

//! Rounds a 55 bit mantissa to 53 bits using round to nearest even, and updates the exponent if necessary
EJ_ALWAYS_INLINE void double_round_to_nearest_even(uint64_t *mantissa, int *exponent, uint64_t remainder) noexcept {
	uint64_t m = *mantissa;
	int e = *exponent;

	//Extract the last bit of the mantissa and combine it with the remainder
	remainder |= m & 4;
	//Update the last bit of the mantissa based on the remainder, and use addition to implement rounding
	m |= remainder != 0 ? 1 : 0;
	++m;
	m >>= 2;
	//In the unlikely event that the mantissa is all ones, then the rounding will increase the size of the mantissa by one bit
	//Shift the mantissa back to the correct position in that case
	if ((m & (static_cast<uint64_t>(1) << DBL_MANT_DIG)) != 0) {
		m >>= 1;
		++e;
	}

	*mantissa = m;
	*exponent = e;
}

EJ_ALWAYS_INLINE double make_double(uint64_t mantissa, int exponent, bool negative) noexcept {
	union {
		uint64_t u;
		double d;
	} ud;

	ud.u = (static_cast<uint64_t>(negative) << (sizeof(uint64_t) * CHAR_BIT - 1)) |
		(static_cast<uint64_t>(exponent + (2 - DBL_MIN_EXP)) << (DBL_MANT_DIG - 1)) |
		(mantissa & ((static_cast<uint64_t>(1) << (DBL_MANT_DIG - 1)) - 1u));
	return ud.d;
}

EJ_ALWAYS_INLINE double make_subnormal_double(uint64_t mantissa, int exponent, uint64_t remainder, bool negative) noexcept {
	int remainder_bits;

	remainder_bits = (DBL_MIN_EXP - 1) - exponent;
	if (remainder_bits <= DBL_MANT_DIG) {
		remainder |= mantissa << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
		mantissa >>= remainder_bits;
		exponent = DBL_MIN_EXP - 2;

		//Round to nearest even
		remainder |= mantissa & 4;
		mantissa |= remainder != 0 ? 1 : 0;
		++mantissa;
		mantissa >>= 2;
		if ((mantissa & (UINT64_C(1) << (DBL_MANT_DIG - 1))) != 0) {
			exponent = DBL_MIN_EXP - 1;
		}

		return make_double(mantissa, exponent, negative);
	} else {
		return !negative ? 0. : -0.;
	}
}

EJ_ALWAYS_INLINE void tuint64_mul(uint64_t *r_0, uint64_t *r_1, uint64_t *r_2, duint64 a, uint64_t b) noexcept {
	duint64 low_product, high_product;

	low_product = duint64_mul(duint64_get_low(a), b);
	high_product = duint64_add(duint64_mul(duint64_get_high(a), b), duint64_get_high(low_product));

	*r_0 = duint64_get_low(low_product);
	*r_1 = duint64_get_low(high_product);
	*r_2 = duint64_get_high(high_product);
}

EJ_ALWAYS_INLINE void tuint64_shl(uint64_t *r_0, uint64_t *r_1, uint64_t *r_2, uint64_t a, unsigned n) noexcept {
	assert(n > 0 && n < sizeof(uint64_t) * CHAR_BIT * 3);

	if (n < sizeof(uint64_t) * CHAR_BIT) {
		*r_0 = a << n;
		*r_1 = a >> (sizeof(uint64_t) * CHAR_BIT - n);
		*r_2 = 0;
	} else if (n < sizeof(uint64_t) * CHAR_BIT * 2) {
		*r_0 = 0;
		if (n > sizeof(uint64_t) * CHAR_BIT) {
			*r_1 = a << (n - sizeof(uint64_t) * CHAR_BIT);
			*r_2 = a >> (sizeof(uint64_t) * CHAR_BIT * 2 - n);
		} else {
			*r_1 = a;
			*r_2 = 0;
		}
	} else {
		*r_0 = 0;
		*r_1 = 0;
		*r_2 = a << (n - sizeof(uint64_t) * CHAR_BIT * 2);
	}
}

EJ_ALWAYS_INLINE bool tuint64_sub_assign(uint64_t *a_0, uint64_t *a_1, uint64_t *a_2, uint64_t b_0, uint64_t b_1, uint64_t b_2) noexcept {
	unsigned long long t_0, t_1, t_2;

	auto borrow = _subborrow_u64(_subborrow_u64(__builtin_sub_overflow(*a_0, b_0, &t_0), *a_1, b_1, &t_1), *a_2, b_2, &t_2);
	*a_0 = t_0;
	*a_1 = t_1;
	*a_2 = t_2;
	return static_cast<bool>(borrow);
}

EJ_ALWAYS_INLINE int small_ceil_log2_5_e(int exponent) noexcept {
	assert(exponent >= 0 && exponent <= 788);
	return exponent * 304339 / 131072 + 1;
}

EJ_ALWAYS_INLINE int small_floor_log2_5_e(int exponent) noexcept {
	assert(exponent >= 0 && exponent <= 788);
	return exponent * 304339 / 131072;
}

float decimal_to_float(uint32_t significand, int exponent, bool negative) noexcept {
	constexpr unsigned max_exponent_of_five = 38;
	constexpr unsigned max_reciprocal_exponent_of_five = 54;
	duint64 product128, product128_ub;
	uint64_t remainder64, mantissa64, difference64, remainder64_ub, mantissa64_ub;
	uint32_t mantissa32;
	int mantissa_bits_less_1, remainder_bits;
	int final_exponent, reciprocal_exponent;

	if (significand != 0) {
		if (exponent >= 0) {
			auto abs_exponent = static_cast<unsigned>(exponent);
			//Float arithmetic should be strictly IEEE compliant for the following to get a correct rounded result,
			//so do a simple test to determine if the target architecture supports it
#if !defined(__i386__) || (defined(__SSE_MATH__) && !defined(__FAST_MATH__))
			if (abs_exponent < get_size(FloatPowersOf10) && significand < (1 << FLT_MANT_DIG)) {
				int32_t signed_significand = !negative ? static_cast<int32_t>(significand) : -static_cast<int32_t>(significand);
				return static_cast<float>(signed_significand) * FloatPowersOf10[abs_exponent];
			}
#endif

			if (EJ_LIKELY(abs_exponent < EJ_FIRST_ESTIMATED_POWER_OF_FIVE_UINT64)) {
				product128 = duint64_mul(static_cast<uint64_t>(significand), NormalizedEstimatedPowersOfFiveUint64[abs_exponent]);
				remainder64 = duint64_get_low(product128);
				mantissa64 = duint64_get_high(product128);
				auto bsr_rv = bsr64(mantissa64);
				if (!bsr_rv.ZeroValue) {
					mantissa_bits_less_1 = static_cast<int>(bsr_rv.Count);
					final_exponent = exponent + mantissa_bits_less_1 + sizeof(uint64_t) * CHAR_BIT;

					remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);
					if (remainder_bits > 0) {
						remainder64 |= mantissa64 << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
						mantissa64 >>= remainder_bits;
					} else if (remainder_bits < 0) {
						duint64_small_shl_assign(&remainder64, &mantissa64, -remainder_bits);
					}
				} else {
					mantissa64 = remainder64;
					remainder64 = 0;

					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa64));
					final_exponent = exponent + mantissa_bits_less_1;
					remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);
					if (remainder_bits > 0) {
						remainder64 = mantissa64 << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
						mantissa64 >>= remainder_bits;
					} else {
						mantissa64 <<= -remainder_bits;
					}
				}
				mantissa32 = static_cast<uint32_t>(mantissa64);
				float_round_to_nearest_even(&mantissa32, &final_exponent, remainder64);
				return make_float(mantissa32, final_exponent, negative);
			} else if (abs_exponent <= max_exponent_of_five) {
				if (significand != 1) {
					product128 = duint64_mul(static_cast<uint64_t>(significand), NormalizedEstimatedPowersOfFiveUint64[abs_exponent]);
					remainder64 = duint64_get_low(product128);
					mantissa64 = duint64_get_high(product128);
					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa64));
					remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);
					product128_ub = duint64_add(product128, significand);
					remainder64_ub = duint64_get_low(product128_ub);
					mantissa64_ub = duint64_get_high(product128_ub);
					if (remainder_bits > 0) {
						remainder64 |= mantissa64 << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
						mantissa64 >>= remainder_bits;
						mantissa64_ub >>= remainder_bits;
					} else if (remainder_bits < 0) {
						remainder_bits = -remainder_bits;
						mantissa64 = (mantissa64 << remainder_bits) | (remainder64 >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
						mantissa64_ub = (mantissa64_ub << remainder_bits) | (remainder64_ub >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
					}
					assert(mantissa64 >= (1 << (FLT_MANT_DIG + 1)) && mantissa64 < (1 << (FLT_MANT_DIG + 2)));
					assert(mantissa64_ub >= (1 << (FLT_MANT_DIG + 1)) && mantissa64_ub <= (1 << (FLT_MANT_DIG + 2)));

					if (EJ_LIKELY(mantissa64 == mantissa64_ub)) {
						final_exponent = exponent + small_ceil_log2_5_e(exponent) + mantissa_bits_less_1;
						remainder64 = 1;
					} else {
						auto power_of_five_of_low_bits = NormalizedEstimatedPowersOfFiveUint64[abs_exponent & EJ_MAX_SMALL_POWER_OF_FIVE_MASK];
						if (abs_exponent < 32) {
							product128 = duint64_mul(152587890625u, power_of_five_of_low_bits);
						} else {
							product128 = duint64_mul(make_duint64(UINT64_C(3273344365508751233), UINT64_C(1262)), power_of_five_of_low_bits);
						}
						duint64_mul_assign(&product128, significand);
						remainder64 = duint64_get_low(product128);
						mantissa64 = duint64_get_high(product128);
						assert(mantissa64 > 0);
						mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa64));
						final_exponent = exponent + mantissa_bits_less_1 + sizeof(uint64_t) * CHAR_BIT;
						remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);
						if (remainder_bits > 0) {
							remainder64 |= mantissa64 << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
							mantissa64 >>= remainder_bits;
						} else if (remainder_bits < 0) {
							duint64_small_shl_assign(&remainder64, &mantissa64, -remainder_bits);
						}
					}
				} else {
					mantissa64 = NormalizedEstimatedPowersOfFiveUint64[abs_exponent] >> (sizeof(uint64_t) * CHAR_BIT - (FLT_MANT_DIG + 2));
					assert(mantissa64 >= (1 << (FLT_MANT_DIG + 1)) && mantissa64 < (1 << (FLT_MANT_DIG + 2)));
					remainder64 = 1;
					final_exponent = exponent + small_floor_log2_5_e(abs_exponent);
				}

				mantissa32 = static_cast<uint32_t>(mantissa64);
				float_round_to_nearest_even(&mantissa32, &final_exponent, remainder64);
				if (final_exponent < FLT_MAX_EXP) {
					return make_float(mantissa32, final_exponent, negative);
				} else {
					return !negative ? HUGE_VALF : -HUGE_VALF;
				}
			} else {
				return !negative ? HUGE_VALF : -HUGE_VALF;
			}
		} else {
			auto abs_exponent = static_cast<unsigned>(-exponent);
			//Float arithmetic should be strictly IEEE compliant for the following to get a correct rounded result,
			//so do a simple test to determine if the target architecture supports it
#if !defined(__i386__) || (defined(__SSE_MATH__) && !defined(__FAST_MATH__))
			if (abs_exponent < get_size(FloatPowersOf10) && significand < (1 << FLT_MANT_DIG)) {
				int32_t signed_significand = !negative ? static_cast<int32_t>(significand) : -static_cast<int32_t>(significand);
				return static_cast<float>(signed_significand) / FloatPowersOf10[abs_exponent];
			}
#endif

			if (abs_exponent <= max_reciprocal_exponent_of_five) {
				if (significand != 1) {
					product128 = duint64_mul(static_cast<uint64_t>(significand), NormalizedReciprocalsOfFiveUint64[abs_exponent - 1]);
					remainder64 = duint64_get_low(product128);
					mantissa64 = duint64_get_high(product128);
					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa64));
					remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);
					reciprocal_exponent = small_floor_log2_5_e(abs_exponent);
					final_exponent = exponent - reciprocal_exponent + mantissa_bits_less_1;
					reciprocal_exponent += sizeof(uint64_t) * CHAR_BIT;
					if (EJ_LIKELY(abs_exponent < EJ_FIRST_ESTIMATED_POWER_OF_FIVE_UINT64)) {
						if (remainder_bits < 0) {
							remainder_bits = -remainder_bits;
							mantissa64 = (mantissa64 << remainder_bits) | (remainder64 >> (sizeof(remainder64) * CHAR_BIT - remainder_bits));
							remainder_bits = sizeof(remainder64) * CHAR_BIT - remainder_bits;
						} else {
							mantissa64 >>= remainder_bits;
							remainder_bits += sizeof(remainder64) * CHAR_BIT;
						}
						assert(mantissa64 >= (1 << (FLT_MANT_DIG + 1)) && mantissa64 < (1 << (FLT_MANT_DIG + 2)));

						auto denominator = NormalizedEstimatedPowersOfFiveUint64[abs_exponent];
						remainder_bits = reciprocal_exponent - remainder_bits;
						if (remainder_bits > 0) {
							auto numerator = duint64_shl(static_cast<uint64_t>(significand), remainder_bits);
							remainder64 = 0;
							auto qd = duint64_mul(mantissa64, denominator);
							auto full_difference = duint64_sub(numerator, qd);
							difference64 = duint64_get_low(full_difference);
							assert(duint64_get_high(full_difference) == 0);
						} else if (remainder_bits < 0) {
							auto numerator = significand;
							remainder64 = numerator << (sizeof(uint64_t) * CHAR_BIT + remainder_bits);
							numerator >>= -remainder_bits;
							assert(numerator >= duint64_mul(mantissa64, denominator) && mantissa64 <= UINT32_MAX && denominator <= UINT32_MAX);
							difference64 = numerator - static_cast<uint32_t>(mantissa64) * static_cast<uint32_t>(denominator);
						} else {
							remainder64 = 0;
							assert(significand >= duint64_mul(mantissa64, denominator) && mantissa64 <= UINT32_MAX && denominator <= UINT32_MAX);
							difference64 = significand - static_cast<uint32_t>(mantissa64) * static_cast<uint32_t>(denominator);
						}
						mantissa32 = static_cast<uint32_t>(mantissa64);
						if (difference64 >= denominator) {
							++mantissa32;
							if (mantissa32 >= (1 << (FLT_MANT_DIG + 2))) {
								assert((mantissa32 % 2) == 0);
								mantissa32 >>= 1;
								++final_exponent;
							}
							difference64 -= denominator;
							assert(difference64 < denominator);
						}
						assert(mantissa32 >= (1 << (FLT_MANT_DIG + 1)) && mantissa32 < (1 << (FLT_MANT_DIG + 2)));
						remainder64 |= difference64;

						float_round_to_nearest_even(&mantissa32, &final_exponent, remainder64);
						return make_float(mantissa32, final_exponent, negative);
					} else {
						product128_ub = duint64_add(product128, significand);
						remainder64_ub = duint64_get_low(product128_ub);
						mantissa64_ub = duint64_get_high(product128_ub);
						if (remainder_bits < 0) {
							remainder_bits = -remainder_bits;
							mantissa64 = (mantissa64 << remainder_bits) | (remainder64 >> (sizeof(remainder64) * CHAR_BIT - remainder_bits));
							mantissa64_ub = (mantissa64_ub << remainder_bits) | (remainder64_ub >> (sizeof(remainder64) * CHAR_BIT - remainder_bits));
							remainder_bits = sizeof(remainder64) * CHAR_BIT - remainder_bits;
						} else {
							mantissa64 >>= remainder_bits;
							mantissa64_ub >>= remainder_bits;
							remainder_bits += sizeof(remainder64) * CHAR_BIT;
						}
						assert(mantissa64 >= (1 << (FLT_MANT_DIG + 1)) && mantissa64 < (1 << (FLT_MANT_DIG + 2)));
						assert(mantissa64_ub >= (1 << (FLT_MANT_DIG + 1)) && mantissa64_ub <= (1 << (FLT_MANT_DIG + 2)));

						if (EJ_LIKELY(mantissa64 == mantissa64_ub)) {
							mantissa32 = static_cast<uint32_t>(mantissa64);
							remainder64 = 1;
						} else {
							duint64 denominator;
							uint64_t qd_0, qd_1, qd_2;
							uint64_t difference_0, difference_1, difference_2;

							auto power_of_five_of_low_bits = NormalizedEstimatedPowersOfFiveUint64[abs_exponent & EJ_MAX_SMALL_POWER_OF_FIVE_MASK];
							if (abs_exponent < 32) {
								denominator = duint64_mul(152587890625u, power_of_five_of_low_bits);
							} else {
								auto *denominator_start = LargePowersOfFive + LargePowersOfFiveOffsets[(abs_exponent / (1u << EJ_MAX_SMALL_POWER_OF_FIVE_BITS)) - 1];
								denominator = duint64_mul(make_duint64(denominator_start[0], denominator_start[1]), power_of_five_of_low_bits);
							}
							tuint64_mul(&qd_0, &qd_1, &qd_2, denominator, mantissa64);

							tuint64_shl(&difference_0, &difference_1, &difference_2, significand, reciprocal_exponent - remainder_bits);
							tuint64_sub_assign(&difference_0, &difference_1, &difference_2, qd_0, qd_1, qd_2);
							mantissa32 = static_cast<uint32_t>(mantissa64);
							if (!tuint64_sub_assign(&difference_0, &difference_1, &difference_2, duint64_get_low(denominator), duint64_get_high(denominator), 0)) {
								++mantissa32;
								if (mantissa32 >= (1 << (FLT_MANT_DIG + 2))) {
									assert((mantissa32 % 2) == 0);
									mantissa32 >>= 1;
									++final_exponent;
								}
							}
							assert(mantissa32 >= (1 << (FLT_MANT_DIG + 1)) && mantissa32 < (1 << (FLT_MANT_DIG + 2)));
							remainder64 |= difference_0 | difference_1 | difference_2;
						}
					}
				} else {
					mantissa64 = NormalizedReciprocalsOfFiveUint64[abs_exponent - 1] >> (sizeof(uint64_t) * CHAR_BIT - (FLT_MANT_DIG + 2));
					assert(mantissa64 >= (1 << (FLT_MANT_DIG + 1)) && mantissa64 < (1 << (FLT_MANT_DIG + 2)));
					mantissa32 = static_cast<uint32_t>(mantissa64);
					remainder64 = 1;
					final_exponent = exponent - small_floor_log2_5_e(abs_exponent) - 1;
				}

				if (final_exponent >= (FLT_MIN_EXP - 1)) {
					float_round_to_nearest_even(&mantissa32, &final_exponent, remainder64);
					return make_float(mantissa32, final_exponent, negative);
				} else {
					return make_subnormal_float(mantissa32, final_exponent, remainder64, negative);
				}
			} else {
				return !negative ? 0.f : -0.f;
			}
		}
	} else {
		return !negative ? 0.f : -0.f;
	}
}

float binary_to_float(uint32_t significand, int exponent, bool negative) noexcept {
	uint32_t mantissa, remainder;
	int mantissa_bits_less_1, remainder_bits, final_exponent;

	auto rv = bsr32(significand);
	if (!rv.ZeroValue) {
		mantissa_bits_less_1 = rv.Count;
		remainder_bits = mantissa_bits_less_1 - (FLT_MANT_DIG + 1);

		remainder = 0;
		if (remainder_bits <= 0) {
			mantissa = significand << -remainder_bits;
		} else {
			remainder = significand << (sizeof(significand) * CHAR_BIT - remainder_bits);
			mantissa = significand >> remainder_bits;
		}
		final_exponent = exponent + mantissa_bits_less_1;

		//Check if exponent is big enough
		if (final_exponent >= (FLT_MIN_EXP - 1)) {
			//Round and check overflow
			float_round_to_nearest_even(&mantissa, &final_exponent, remainder);
			if (final_exponent < FLT_MAX_EXP) {
				return make_float(mantissa, final_exponent, negative);
			} else {
				return !negative ? HUGE_VALF : -HUGE_VALF;
			}
		} else {
			return make_subnormal_float(mantissa, final_exponent, remainder, negative);
		}
	} else {
		return !negative ? 0.f : -0.f;
	}
}

StringToReturnType<float> c_string_to_float(const char *s) noexcept {
	const char *dot_next_ch_ptr = nullptr;
	int fraction_exponent, exponent;
	uint32_t value;
	unsigned digit;
	unsigned char ch;
	bool negate, negate_exponent;

	//Checks for a negative sign
	ch = static_cast<unsigned char>(*s);
	s++;
	negate = ch == '-';
	if (!negate) {
		goto action_skip_zero;
	}

	ch = static_cast<unsigned char>(*s);
	s++;
action_skip_zero:
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_leading_zero;
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('.') - '0'))) {
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_fraction_skip_zero;
		} else {
			return { static_cast<float>(0), false };
		}
	} else {
		return { static_cast<float>(0), false };
	}

state_fraction_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_optional_skip_zeros;
		}
	} else {
		return { static_cast<float>(0), false };
	}

state_leading_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '.':
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_optional_skip_zeros;
		} else {
			return { static_cast<float>(0), false };
		}

	case 'E':
	case 'e':
		value = 0;
		fraction_exponent = 0;
		goto state_exponent_optional_sign;

	case 'x':
		goto state_hex_skip_first_zero;

	case '\0':
		return { static_cast<float>(0), true };

	default:
		return { static_cast<float>(0), false };
	}

state_optional_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_optional_skip_zeros;
		}
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_skip_zeros;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			value = 0;
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			return { static_cast<float>(0), true };

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_2_set_value:
	value = digit;

state_optional_digit_2:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_2;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_3:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_3;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_4:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_4;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_5:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_5;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_6:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_6;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_7:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_7;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_8:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_8;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_9:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_9;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_optional_digit_10:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(value < UINT32_C(429496729)) || (value == UINT32_C(429496729) && digit <= 5)) {
			value = value * 10 + digit;

			ch = static_cast<unsigned char>(*s);
			s++;
			switch (ch) {
			case '.':
				if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
					ch = static_cast<unsigned char>(*s);
					s++;
					switch (ch) {
					case 'E':
					case 'e':
						fraction_exponent = 0;
						goto state_exponent_optional_sign;

					case '\0':
						return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };

					default:
						return { static_cast<float>(0), false };
					}
				} else {
					return{ static_cast<float>(0), false };
				}

			case 'E':
			case 'e':
				if (dot_next_ch_ptr != nullptr) {
					fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				} else {
					fraction_exponent = 0;
				}
				goto state_exponent_optional_sign;

			case '\0':
				if (dot_next_ch_ptr != nullptr) {
					fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
					return { decimal_to_float(value, fraction_exponent, negate), true };
				} else {
					return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
				}

			default:
				return { static_cast<float>(0), false };
			}
		} else {
			return { static_cast<float>(0), false };
		}
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_10;
			} else {
				return { static_cast<float>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(!negate ? static_cast<int32_t>(value) : -static_cast<int32_t>(value)), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_exponent_optional_sign:
	negate_exponent = false;
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(0), false };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - '0'))) {
		negate_exponent = true;
		goto state_exponent_skip_zero;
	} else {
		return { static_cast<float>(0), false };
	}

state_exponent_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { decimal_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(0), false };
			}
		}
	} else {
		return { static_cast<float>(0), false };
	}

state_exponent_optional_digits:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(exponent <= (INT_MAX / 10 - 1))) {
			exponent = exponent * 10 + digit;
		}
		goto state_exponent_optional_digits;
	} else if (EJ_LIKELY(digit == -static_cast<unsigned>('0'))) {
		return { decimal_to_float(value, (!negate_exponent ? exponent : -exponent) + fraction_exponent, negate), true };
	} else {
		return { static_cast<float>(0), false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		if (digit > 0) {
			goto state_hex_digit_2_set_value;
		} else {
			goto state_hex_skip_zeros;
		}
	} else if (EJ_LIKELY(digit == 0x7e)) {	//'.'
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_hex_skip_zeros;
		} else {
			return { static_cast<float>(0), false };
		}
	} else {
		return { static_cast<float>(0), false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		if (digit > 0) {
			goto state_hex_digit_2_set_value;
		} else {
			goto state_hex_skip_zeros;
		}
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			value = 0;
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_skip_zeros;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:
			return { static_cast<float>(0), true };

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_2_set_value:
	value = digit;

state_hex_digit_2:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_2;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_3:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_3;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_4:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_4;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_5:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_5;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_6:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_6;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_7:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_7;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

state_hex_digit_8:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_8;
			} else {
				return { static_cast<float>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
			}

		default:
			return { static_cast<float>(0), false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch)
	{
	case 'P':
	case 'p':
		if (dot_next_ch_ptr != nullptr) {
			fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
		} else {
			fraction_exponent = 0;
		}
		goto state_binary_exponent_optional_sign;

	case '.':
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			ch = static_cast<unsigned char>(*s);
			switch (ch) {
			case '\0':
				return { binary_to_float(value, 0, negate), true };

			case 'P':
			case 'p':
				s++;
				fraction_exponent = 0;
				goto state_binary_exponent_optional_sign;

			default:
				return { static_cast<float>(0), false };
			}
		} else {
			return { static_cast<float>(0), false };
		}

	case '\0':
		if (dot_next_ch_ptr != nullptr) {
			fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			return { binary_to_float(value, fraction_exponent, negate), true };
		} else {
			return { !negate ? static_cast<float>(value) : -static_cast<float>(value), true };
		}

	default:
		return { static_cast<float>(0), false };
	}

state_binary_exponent_optional_sign:
	negate_exponent = false;
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_binary_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(0), false };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - '0'))) {
		negate_exponent = true;
		goto state_binary_exponent_skip_zero;
	} else {
		return { static_cast<float>(0), false };
	}

state_binary_exponent_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_binary_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { binary_to_float(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<float>(0), false };
			}
		}
	} else {
		return { static_cast<float>(0), false };
	}

state_binary_exponent_optional_digits:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(exponent <= (INT_MAX / 10 - 1))) {
			exponent = exponent * 10 + digit;
		}
		goto state_binary_exponent_optional_digits;
	} else if (EJ_LIKELY(digit == -static_cast<unsigned>('0'))) {
		return { binary_to_float(value, (!negate_exponent ? exponent : -exponent) + fraction_exponent, negate), true };
	} else {
		return { static_cast<float>(0), false };
	}
}

static const double DoublePowersOf10[] = {
	1e0,
	1e1,
	1e2,
	1e3,
	1e4,
	1e5,
	1e6,
	1e7,
	1e8,
	1e9,
	1e10,
	1e11,
	1e12,
	1e13,
	1e14,
	1e15,
	1e16,
	1e17,
	1e18,
	1e19,
	1e20,
	1e21,
	1e22,
};

//The third parameter uses rdx in the API calling convention, which maps to the implicit input in the mulx instruction
uint64_t *decimal_to_double_mp_uint64_mul(uint64_t *d, const uint64_t *a, uint64_t b, const uint64_t *a_end) noexcept {
	duint64 p;
	uintptr_t offset, max_offset;
	uint64_t carry;

	assert((d <= a || d >= a_end) && a_end - a >= 1);

	p = duint64_mul(*a, b);
	*d = duint64_get_low(p);
	carry = duint64_get_high(p);
	offset = sizeof(*a);
	max_offset = reinterpret_cast<uintptr_t>(a_end) - reinterpret_cast<uintptr_t>(a);

	for (; offset < max_offset; offset += sizeof(*a)) {
		p = duint64_add(duint64_mul(*lea(a, offset), b), carry);
		*lea(d, offset) = duint64_get_low(p);
		carry = duint64_get_high(p);
	}

	auto *d_i = lea(d, max_offset);
	*d_i = carry;
	if (carry != 0) {
		++d_i;
	}
	return d_i;
}

inline bool decimal_to_double_mp_uint64_is_zero(const uint64_t *a, const uint64_t *a_end) noexcept {
	for (const auto *a_i = a; a_i < a_end; a_i++) {
		if (*a_i != 0) {
			return false;
		}
	}

	return true;
}

double decimal_to_double(uint64_t significand, int exponent, bool negative) noexcept {
	constexpr unsigned max_exponent_of_five = get_size(NormalizedEstimatedPowersOfFiveUint64) - 1;
	static_assert(max_exponent_of_five == 308);
	constexpr unsigned max_reciprocal_exponent_of_five = get_size(NormalizedReciprocalsOfFiveUint64);
	static_assert(max_reciprocal_exponent_of_five == 342);
	constexpr unsigned max_mp_uints = 1024 / (sizeof(uint64_t) * CHAR_BIT);
	duint64 product, product_ub;
	uint64_t remainder, mantissa, difference, remainder_ub, mantissa_ub;
	int mantissa_bits_less_1, remainder_bits;
	int final_exponent, reciprocal_exponent;

	if (significand != 0) {
		if (exponent >= 0) {
			auto abs_exponent = static_cast<unsigned>(exponent);
			//Double arithmetic should be strictly IEEE compliant for the following to get a correct rounded result
			if (abs_exponent < get_size(DoublePowersOf10) && significand < (UINT64_C(1) << DBL_MANT_DIG)) {
				int64_t signed_significand = !negative ? static_cast<int64_t>(significand) : -static_cast<int64_t>(significand);
				return static_cast<double>(signed_significand) * DoublePowersOf10[abs_exponent];
			}

			if (EJ_LIKELY(abs_exponent < EJ_FIRST_ESTIMATED_POWER_OF_FIVE_UINT64)) {
				product = duint64_mul(significand, NormalizedEstimatedPowersOfFiveUint64[abs_exponent]);
				remainder = duint64_get_low(product);
				mantissa = duint64_get_high(product);
				auto bsr_rv = bsr64(mantissa);
				if (!bsr_rv.ZeroValue) {
					mantissa_bits_less_1 = static_cast<int>(bsr_rv.Count);
					final_exponent = exponent + mantissa_bits_less_1 + sizeof(uint64_t) * CHAR_BIT;
					remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);

					if (remainder_bits > 0) {
						remainder |= mantissa << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
						mantissa >>= remainder_bits;
					} else if (remainder_bits < 0) {
						duint64_small_shl_assign(&remainder, &mantissa, -remainder_bits);
					}
				} else {
					mantissa = remainder;
					remainder = 0;

					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa));
					final_exponent = exponent + mantissa_bits_less_1;
					remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);
					if (remainder_bits > 0) {
						remainder = mantissa << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
						mantissa >>= remainder_bits;
					} else {
						mantissa <<= -remainder_bits;
					}
				}

				double_round_to_nearest_even(&mantissa, &final_exponent, remainder);
				return make_double(mantissa, final_exponent, negative);
			} else if (abs_exponent <= max_exponent_of_five) {
				if (significand != 1) {
					product = duint64_mul(significand, NormalizedEstimatedPowersOfFiveUint64[abs_exponent]);
					remainder = duint64_get_low(product);
					mantissa = duint64_get_high(product);
					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa));
					remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);
					product_ub = duint64_add(product, significand);
					remainder_ub = duint64_get_low(product_ub);
					mantissa_ub = duint64_get_high(product_ub);
					if (remainder_bits < 0) {
						remainder_bits = -remainder_bits;
						mantissa = (mantissa << remainder_bits) | (remainder >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
						mantissa_ub = (mantissa_ub << remainder_bits) | (remainder_ub >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
					} else {
						mantissa >>= remainder_bits;
						mantissa_ub >>= remainder_bits;
					}
					assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
					assert(mantissa_ub >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa_ub <= (UINT64_C(1) << (DBL_MANT_DIG + 2)));

					if (EJ_LIKELY(mantissa == mantissa_ub)) {
						final_exponent = exponent + small_ceil_log2_5_e(exponent) + mantissa_bits_less_1;
						remainder = 1;
					} else {
						uint64_t full_product[max_mp_uints];
						uint64_t *full_product_end;

						full_product_end = decimal_to_double_mp_uint64_mul(
							full_product,
							LargePowersOfFive + LargePowersOfFiveOffsets[(abs_exponent / (1u << EJ_MAX_SMALL_POWER_OF_FIVE_BITS)) - 1],
							NormalizedEstimatedPowersOfFiveUint64[abs_exponent & EJ_MAX_SMALL_POWER_OF_FIVE_MASK],
							LargePowersOfFive + LargePowersOfFiveOffsets[(abs_exponent / (1u << EJ_MAX_SMALL_POWER_OF_FIVE_BITS))]);
						full_product_end = decimal_to_double_mp_uint64_mul(full_product, full_product, significand, full_product_end);

						remainder = *(full_product_end - 2);
						mantissa = *(full_product_end - 1);
						mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa));
						//Compute the exponent
						final_exponent = exponent + mantissa_bits_less_1 + ((full_product_end - full_product) - 1) * sizeof(uint64_t) * CHAR_BIT;
						//Normalize the mantissa and remainder
						remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);
						if (remainder_bits > 0) {
							//Parts of the mantissa belongs to the remainder
							remainder |= mantissa << (sizeof(uint64_t) * CHAR_BIT - remainder_bits);
							mantissa >>= remainder_bits;
						} else if (remainder_bits < 0) {
							//Parts of the remainders belongs to the mantissa
							duint64_small_shl_assign(&remainder, &mantissa, -remainder_bits);
						}
						assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
						//Combine the rest of the remainder
						for (auto full_product_i = full_product; full_product_i < (full_product_end - 2); ++full_product_i) {
							remainder |= *full_product_i;
						}
					}
				} else {
					mantissa = NormalizedEstimatedPowersOfFiveUint64[abs_exponent] >> (sizeof(uint64_t) * CHAR_BIT - (DBL_MANT_DIG + 2));
					assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
					remainder = 1;
					final_exponent = exponent + small_ceil_log2_5_e(exponent);
				}

				//Round and return the result
				double_round_to_nearest_even(&mantissa, &final_exponent, remainder);
				if (final_exponent < DBL_MAX_EXP) {
					return make_double(mantissa, final_exponent, negative);
				} else {
					return !negative ? HUGE_VAL : -HUGE_VAL;
				}
			} else {
				return !negative ? HUGE_VAL : -HUGE_VAL;
			}
		} else {
			auto abs_exponent = static_cast<unsigned>(-exponent);
			//Double arithmetic should be strictly IEEE compliant for the following to get a correct rounded result
			if (abs_exponent < get_size(DoublePowersOf10) && significand < (UINT64_C(1) << DBL_MANT_DIG)) {
				int64_t signed_significand = !negative ? static_cast<int64_t>(significand) : -static_cast<int64_t>(significand);
				return static_cast<double>(signed_significand) / DoublePowersOf10[abs_exponent];
			}

			if (abs_exponent <= max_reciprocal_exponent_of_five) {
				if (significand != 1) {
					product = duint64_mul(significand, NormalizedReciprocalsOfFiveUint64[abs_exponent - 1]);
					remainder = duint64_get_low(product);
					mantissa = duint64_get_high(product);
					mantissa_bits_less_1 = static_cast<int>(bsr64_nz(mantissa));
					remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);
					reciprocal_exponent = small_floor_log2_5_e(abs_exponent);
					final_exponent = exponent - reciprocal_exponent + mantissa_bits_less_1;
					reciprocal_exponent += sizeof(uint64_t) * CHAR_BIT;
					if (EJ_LIKELY(abs_exponent < EJ_FIRST_ESTIMATED_POWER_OF_FIVE_UINT64)) {
						if (remainder_bits < 0) {
							remainder_bits = -remainder_bits;
							mantissa = (mantissa << remainder_bits) | (remainder >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
							remainder_bits = sizeof(uint64_t) * CHAR_BIT - remainder_bits;
						} else {
							mantissa >>= remainder_bits;
							remainder_bits += sizeof(uint64_t) * CHAR_BIT;
						}
						assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));

						auto denominator = NormalizedEstimatedPowersOfFiveUint64[abs_exponent];
						remainder_bits = reciprocal_exponent - remainder_bits;
						if (remainder_bits > 0) {
							auto numerator = duint64_shl(significand, remainder_bits);
							remainder = 0;
							auto qd = duint64_mul(mantissa, denominator);
							auto full_difference = duint64_sub(numerator, qd);
							difference = duint64_get_low(full_difference);
							assert(duint64_get_high(full_difference) == 0);
						} else if (remainder_bits < 0) {
							auto numerator = significand;
							remainder = numerator << (sizeof(uint64_t) * CHAR_BIT + remainder_bits);
							numerator >>= -remainder_bits;
							assert(numerator >= duint64_mul(mantissa, denominator));
							difference = numerator - mantissa * denominator;
						} else {
							remainder = 0;
							difference = significand - mantissa * denominator;
						}

						if (difference >= denominator) {
							++mantissa;
							if (mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 2))) {
								assert((mantissa % 2) == 0);
								mantissa >>= 1;
								++final_exponent;
							}
							difference -= denominator;
							assert(difference < denominator);
						}
						assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
						remainder |= difference;

						double_round_to_nearest_even(&mantissa, &final_exponent, remainder);
						return make_double(mantissa, final_exponent, negative);
					} else {
						product_ub = duint64_add(product, significand);
						remainder_ub = duint64_get_low(product_ub);
						mantissa_ub = duint64_get_high(product_ub);
						if (remainder_bits < 0) {
							remainder_bits = -remainder_bits;
							mantissa = (mantissa << remainder_bits) | (remainder >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
							mantissa_ub = (mantissa_ub << remainder_bits) | (remainder_ub >> (sizeof(uint64_t) * CHAR_BIT - remainder_bits));
							remainder_bits = sizeof(uint64_t) * CHAR_BIT - remainder_bits;
						} else {
							mantissa >>= remainder_bits;
							mantissa_ub >>= remainder_bits;
							remainder_bits += sizeof(uint64_t) * CHAR_BIT;
						}
						assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
						assert(mantissa_ub >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa_ub <= (UINT64_C(1) << (DBL_MANT_DIG + 2)));

						if (EJ_LIKELY(mantissa == mantissa_ub)) {
							remainder = 1;
						} else {
							uint64_t full_estimate[max_mp_uints];
							uint64_t *full_estimate_end;
							uint64_t *estimate_integer;
							uint64_t estimate, estimate_fraction_mask;
							unsigned estimate_bits;
							unsigned estimate_fraction_bits;
							unsigned estimate_integer_bits;
							unsigned estimate_shr_bits;

							full_estimate_end = decimal_to_double_mp_uint64_mul(
								full_estimate,
								LargePowersOfFive + LargePowersOfFiveOffsets[(abs_exponent / (1u << EJ_MAX_SMALL_POWER_OF_FIVE_BITS)) - 1],
								NormalizedEstimatedPowersOfFiveUint64[abs_exponent & EJ_MAX_SMALL_POWER_OF_FIVE_MASK],
								LargePowersOfFive + LargePowersOfFiveOffsets[(abs_exponent / (1u << EJ_MAX_SMALL_POWER_OF_FIVE_BITS))]);
							full_estimate_end = decimal_to_double_mp_uint64_mul(full_estimate, full_estimate, mantissa + 1, full_estimate_end);

							assert(*(full_estimate_end - 1) != 0);

							estimate_bits = static_cast<unsigned>(bsr64_nz(*(full_estimate_end - 1))) + static_cast<unsigned>(full_estimate_end - full_estimate) * sizeof(estimate) * CHAR_BIT - sizeof(estimate) * CHAR_BIT + 1;
							estimate_fraction_bits = reciprocal_exponent - remainder_bits;
							estimate_integer_bits = estimate_bits - estimate_fraction_bits;
							if (EJ_LIKELY(estimate_integer_bits <= (sizeof(estimate) * CHAR_BIT))) {
								estimate_integer = full_estimate + estimate_fraction_bits / (sizeof(estimate) * CHAR_BIT);
								estimate = *estimate_integer;
								auto *estimate_fraction_end = estimate_integer + 1;
								estimate_shr_bits = estimate_fraction_bits % (sizeof(estimate) * CHAR_BIT);
								if (estimate_shr_bits > 0) {
									estimate_fraction_mask = (UINT64_C(1) << estimate_shr_bits) - 1;
									*estimate_integer = estimate & estimate_fraction_mask;
									estimate >>= estimate_shr_bits;
									if (estimate_fraction_end < full_estimate_end) {
										estimate |= *(full_estimate_end - 1) << (sizeof(estimate) * CHAR_BIT - estimate_shr_bits);
									}
									full_estimate_end = estimate_fraction_end + 1;
								} else {
									full_estimate_end = estimate_integer;
								}

								if (significand > estimate) {
									++mantissa;
									if (mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 2))) {
										assert((mantissa % 2) == 0);
										mantissa >>= 1;
										++final_exponent;
									}
									remainder = 1;
								} else if (significand == estimate) {
									if (EJ_LIKELY(!decimal_to_double_mp_uint64_is_zero(full_estimate, full_estimate_end))) {
										remainder = 1;
									} else {
										++mantissa;
										if (mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 2))) {
											assert((mantissa % 2) == 0);
											mantissa >>= 1;
											++final_exponent;
										}
										remainder = 0;
									}
								} else {
									remainder = 1;
								}
							} else {
								remainder = 1;
							}
							assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
						}
					}
				} else {
					mantissa = NormalizedReciprocalsOfFiveUint64[abs_exponent - 1] >> (sizeof(uint64_t) * CHAR_BIT - (DBL_MANT_DIG + 2));
					assert(mantissa >= (UINT64_C(1) << (DBL_MANT_DIG + 1)) && mantissa < (UINT64_C(1) << (DBL_MANT_DIG + 2)));
					remainder = 1;
					final_exponent = exponent - small_floor_log2_5_e(abs_exponent) - 1;
				}

				if (final_exponent >= (DBL_MIN_EXP - 1)) {
					double_round_to_nearest_even(&mantissa, &final_exponent, remainder);
					return make_double(mantissa, final_exponent, negative);
				} else {
					return make_subnormal_double(mantissa, final_exponent, remainder, negative);
				}
			} else {
				return !negative ? 0. : -0.;
			}
		}
	} else {
		return !negative ? 0. : -0.;
	}
}

double binary_to_double(uint64_t significand, int exponent, bool negative) noexcept {
	uint64_t mantissa, remainder;
	int mantissa_bits_less_1, remainder_bits, final_exponent;

	auto rv = bsr64(significand);
	if (!rv.ZeroValue) {
		mantissa_bits_less_1 = static_cast<int>(rv.Count);
		remainder_bits = mantissa_bits_less_1 - (DBL_MANT_DIG + 1);

		remainder = 0;
		if (remainder_bits <= 0) {
			mantissa = significand << -remainder_bits;
		} else {
			remainder = significand << (sizeof(significand) * CHAR_BIT - remainder_bits);
			mantissa = significand >> remainder_bits;
		}
		final_exponent = exponent + mantissa_bits_less_1;

		//Check if exponent is big enough
		if (final_exponent >= (DBL_MIN_EXP - 1)) {
			//Round and check overflow
			double_round_to_nearest_even(&mantissa, &final_exponent, remainder);
			if (final_exponent < DBL_MAX_EXP) {
				return make_double(mantissa, final_exponent, negative);
			} else {
				return !negative ? HUGE_VAL : -HUGE_VAL;
			}
		} else {
			return make_subnormal_double(mantissa, final_exponent, remainder, negative);
		}
	} else {
		return !negative ? 0. : -0.;
	}
}

StringToReturnType<double> c_string_to_double(const char *s) noexcept {
	const char *dot_next_ch_ptr = nullptr;
	int fraction_exponent, exponent;
	uint64_t value;
	unsigned digit;
	unsigned char ch;
	bool negate, negate_exponent;

	//Checks for a negative sign
	ch = static_cast<unsigned char>(*s);
	s++;
	negate = ch == '-';
	if (!negate) {
		goto action_skip_zero;
	}

	ch = static_cast<unsigned char>(*s);
	s++;
action_skip_zero:
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_leading_zero;
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('.') - '0'))) {
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_fraction_skip_zero;
		} else {
			return { static_cast<double>(0), false };
		}
	} else {
		return { static_cast<double>(0), false };
	}

state_fraction_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_optional_skip_zeros;
		}
	} else {
		return { static_cast<double>(0), false };
	}

state_leading_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch) {
	case '.':
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_optional_skip_zeros;
		} else {
			return { static_cast<double>(0), false };
		}

	case 'E':
	case 'e':
		value = 0;
		fraction_exponent = 0;
		goto state_exponent_optional_sign;

	case 'x':
		goto state_hex_skip_first_zero;

	case '\0':
		return { static_cast<double>(0), true };

	default:
		return { static_cast<double>(0), false };
	}

state_optional_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			goto state_optional_digit_2_set_value;
		} else {
			goto state_optional_skip_zeros;
		}
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_skip_zeros;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			value = 0;
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			return { static_cast<double>(0), true };

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_2_set_value:
	value = digit;

state_optional_digit_2:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_2;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_3:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_3;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_4:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_4;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_5:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_5;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_6:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_6;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_7:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_7;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_8:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_8;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_9:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_9;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_10:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_10;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_11:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_11;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_12:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_12;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_13:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_13;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_14:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_14;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_15:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_15;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_16:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_16;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_17:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_17;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_18:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_18;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_19:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		value = value * 10 + digit;
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_19;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(!negate ? static_cast<int64_t>(value) : -static_cast<int64_t>(value)), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_optional_digit_20:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(value < UINT64_C(1844674407370955161)) || (value == UINT64_C(1844674407370955161) && digit <= 5)) {
			value = value * 10 + digit;

			ch = static_cast<unsigned char>(*s);
			s++;
			switch (ch) {
			case '.':
				if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
					ch = static_cast<unsigned char>(*s);
					s++;
					switch (ch) {
					case 'E':
					case 'e':
						fraction_exponent = 0;
						goto state_exponent_optional_sign;

					case '\0':
						return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };

					default:
						return { static_cast<double>(0), false };
					}
				} else {
					return{ static_cast<double>(0), false };
				}

			case 'E':
			case 'e':
				if (dot_next_ch_ptr != nullptr) {
					fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				} else {
					fraction_exponent = 0;
				}
				goto state_exponent_optional_sign;

			case '\0':
				if (dot_next_ch_ptr != nullptr) {
					fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
					return { decimal_to_double(value, fraction_exponent, negate), true };
				} else {
					return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
				}

			default:
				return { static_cast<double>(0), false };
			}
		} else {
			return { static_cast<double>(0), false };
		}
	} else {
		switch (digit) {
		case static_cast<unsigned>('.') - '0':
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_optional_digit_20;
			} else {
				return { static_cast<double>(0), false };
			}

		case static_cast<unsigned>('E') - '0':
		case static_cast<unsigned>('e') - '0':
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
			} else {
				fraction_exponent = 0;
			}
			goto state_exponent_optional_sign;

		case -static_cast<unsigned>('0'):
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1);
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_exponent_optional_sign:
	negate_exponent = false;
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(0), false };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - '0'))) {
		negate_exponent = true;
		goto state_exponent_skip_zero;
	} else {
		return { static_cast<double>(0), false };
	}

state_exponent_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { decimal_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(0), false };
			}
		}
	} else {
		return { static_cast<double>(0), false };
	}

state_exponent_optional_digits:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(exponent <= (INT_MAX / 10 - 1))) {
			exponent = exponent * 10 + digit;
		}
		goto state_exponent_optional_digits;
	} else if (EJ_LIKELY(digit == -static_cast<unsigned>('0'))) {
		return { decimal_to_double(value, (!negate_exponent ? exponent : -exponent) + fraction_exponent, negate), true };
	} else {
		return { static_cast<double>(0), false };
	}

state_hex_skip_first_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		if (digit > 0) {
			goto state_hex_digit_2_set_value;
		} else {
			goto state_hex_skip_zeros;
		}
	} else if (EJ_LIKELY(digit == 0x7e)) {	//'.'
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			goto state_hex_skip_zeros;
		} else {
			return { static_cast<double>(0), false };
		}
	} else {
		return { static_cast<double>(0), false };
	}

state_hex_skip_zeros:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		if (digit > 0) {
			goto state_hex_digit_2_set_value;
		} else {
			goto state_hex_skip_zeros;
		}
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			value = 0;
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_skip_zeros;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:
			return { static_cast<double>(0), true };

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_2_set_value:
	value = digit;

state_hex_digit_2:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_2;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_3:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_3;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_4:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_4;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_5:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_5;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_6:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_6;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_7:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_7;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_8:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_8;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_9:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_9;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_10:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_10;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_11:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_11;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_12:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_12;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_13:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_13;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_14:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_14;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_15:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_15;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

state_hex_digit_16:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = HexDigitValues[ch];
	if (EJ_LIKELY(digit <= 15)) {
		value = value * 16 + digit;
	} else {
		switch (digit) {
		case 0x19:	//'P' or 'p'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			} else {
				fraction_exponent = 0;
			}
			goto state_binary_exponent_optional_sign;

		case 0x7e:	//'.'
			if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
				dot_next_ch_ptr = s;
				goto state_hex_digit_16;
			} else {
				return { static_cast<double>(0), false };
			}

		case 0x7f:	//'\0'
			if (dot_next_ch_ptr != nullptr) {
				fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
			}

		default:
			return { static_cast<double>(0), false };
		}
	}

	ch = static_cast<unsigned char>(*s);
	s++;
	switch (ch)
	{
	case 'P':
	case 'p':
		if (dot_next_ch_ptr != nullptr) {
			fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
		} else {
			fraction_exponent = 0;
		}
		goto state_binary_exponent_optional_sign;

	case '.':
		if (EJ_LIKELY(dot_next_ch_ptr == nullptr)) {
			dot_next_ch_ptr = s;
			ch = static_cast<unsigned char>(*s);
			switch (ch) {
			case '\0':
				return { binary_to_double(value, 0, negate), true };

			case 'P':
			case 'p':
				s++;
				fraction_exponent = 0;
				goto state_binary_exponent_optional_sign;

			default:
				return { static_cast<double>(0), false };
			}
		} else {
			return { static_cast<double>(0), false };
		}

	case '\0':
		if (dot_next_ch_ptr != nullptr) {
			fraction_exponent = static_cast<int>(dot_next_ch_ptr - s + 1) * 4;
			return { binary_to_double(value, fraction_exponent, negate), true };
		} else {
			return { !negate ? static_cast<double>(value) : -static_cast<double>(value), true };
		}

	default:
		return { static_cast<double>(0), false };
	}

state_binary_exponent_optional_sign:
	negate_exponent = false;
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_binary_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(0), false };
			}
		}
	} else if (EJ_LIKELY(digit == (static_cast<unsigned>('-') - '0'))) {
		negate_exponent = true;
		goto state_binary_exponent_skip_zero;
	} else {
		return { static_cast<double>(0), false };
	}

state_binary_exponent_skip_zero:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (digit > 0) {
			exponent = digit;
			goto state_binary_exponent_optional_digits;
		} else {
			ch = static_cast<unsigned char>(*s);
			s++;
			if (ch == '\0') {
				return { binary_to_double(value, fraction_exponent, negate), true };
			} else {
				return { static_cast<double>(0), false };
			}
		}
	} else {
		return { static_cast<double>(0), false };
	}

state_binary_exponent_optional_digits:
	ch = static_cast<unsigned char>(*s);
	s++;
	digit = static_cast<unsigned>(ch) - '0';
	if (EJ_LIKELY(digit <= 9)) {
		if (EJ_LIKELY(exponent <= (INT_MAX / 10 - 1))) {
			exponent = exponent * 10 + digit;
		}
		goto state_binary_exponent_optional_digits;
	} else if (EJ_LIKELY(digit == -static_cast<unsigned>('0'))) {
		return { binary_to_double(value, (!negate_exponent ? exponent : -exponent) + fraction_exponent, negate), true };
	} else {
		return { static_cast<double>(0), false };
	}
}

static const uint64_t Uint64PowersOf10[] = {
	UINT64_C(1),
	UINT64_C(10),
	UINT64_C(100),
	UINT64_C(1000),
	UINT64_C(10000),
	UINT64_C(100000),
	UINT64_C(1000000),
	UINT64_C(10000000),
	UINT64_C(100000000),
	UINT64_C(1000000000),
	UINT64_C(10000000000),
	UINT64_C(100000000000),
	UINT64_C(1000000000000),
	UINT64_C(10000000000000),
	UINT64_C(100000000000000),
	UINT64_C(1000000000000000),
	UINT64_C(10000000000000000),
	UINT64_C(100000000000000000),
	UINT64_C(1000000000000000000),
	UINT64_C(10000000000000000000),
};

static const char DecimalDigits_000_999[] alignas(4) = {
	 '0', '0', '0', '\0', '0', '0', '1', '\0', '0', '0', '2', '\0', '0', '0', '3', '\0', '0', '0', '4', '\0', '0', '0', '5', '\0', '0', '0', '6', '\0', '0', '0', '7', '\0', '0', '0', '8', '\0', '0', '0', '9', '\0',
	 '0', '1', '0', '\0', '0', '1', '1', '\0', '0', '1', '2', '\0', '0', '1', '3', '\0', '0', '1', '4', '\0', '0', '1', '5', '\0', '0', '1', '6', '\0', '0', '1', '7', '\0', '0', '1', '8', '\0', '0', '1', '9', '\0',
	 '0', '2', '0', '\0', '0', '2', '1', '\0', '0', '2', '2', '\0', '0', '2', '3', '\0', '0', '2', '4', '\0', '0', '2', '5', '\0', '0', '2', '6', '\0', '0', '2', '7', '\0', '0', '2', '8', '\0', '0', '2', '9', '\0',
	 '0', '3', '0', '\0', '0', '3', '1', '\0', '0', '3', '2', '\0', '0', '3', '3', '\0', '0', '3', '4', '\0', '0', '3', '5', '\0', '0', '3', '6', '\0', '0', '3', '7', '\0', '0', '3', '8', '\0', '0', '3', '9', '\0',
	 '0', '4', '0', '\0', '0', '4', '1', '\0', '0', '4', '2', '\0', '0', '4', '3', '\0', '0', '4', '4', '\0', '0', '4', '5', '\0', '0', '4', '6', '\0', '0', '4', '7', '\0', '0', '4', '8', '\0', '0', '4', '9', '\0',
	 '0', '5', '0', '\0', '0', '5', '1', '\0', '0', '5', '2', '\0', '0', '5', '3', '\0', '0', '5', '4', '\0', '0', '5', '5', '\0', '0', '5', '6', '\0', '0', '5', '7', '\0', '0', '5', '8', '\0', '0', '5', '9', '\0',
	 '0', '6', '0', '\0', '0', '6', '1', '\0', '0', '6', '2', '\0', '0', '6', '3', '\0', '0', '6', '4', '\0', '0', '6', '5', '\0', '0', '6', '6', '\0', '0', '6', '7', '\0', '0', '6', '8', '\0', '0', '6', '9', '\0',
	 '0', '7', '0', '\0', '0', '7', '1', '\0', '0', '7', '2', '\0', '0', '7', '3', '\0', '0', '7', '4', '\0', '0', '7', '5', '\0', '0', '7', '6', '\0', '0', '7', '7', '\0', '0', '7', '8', '\0', '0', '7', '9', '\0',
	 '0', '8', '0', '\0', '0', '8', '1', '\0', '0', '8', '2', '\0', '0', '8', '3', '\0', '0', '8', '4', '\0', '0', '8', '5', '\0', '0', '8', '6', '\0', '0', '8', '7', '\0', '0', '8', '8', '\0', '0', '8', '9', '\0',
	 '0', '9', '0', '\0', '0', '9', '1', '\0', '0', '9', '2', '\0', '0', '9', '3', '\0', '0', '9', '4', '\0', '0', '9', '5', '\0', '0', '9', '6', '\0', '0', '9', '7', '\0', '0', '9', '8', '\0', '0', '9', '9', '\0',
	 '1', '0', '0', '\0', '1', '0', '1', '\0', '1', '0', '2', '\0', '1', '0', '3', '\0', '1', '0', '4', '\0', '1', '0', '5', '\0', '1', '0', '6', '\0', '1', '0', '7', '\0', '1', '0', '8', '\0', '1', '0', '9', '\0',
	 '1', '1', '0', '\0', '1', '1', '1', '\0', '1', '1', '2', '\0', '1', '1', '3', '\0', '1', '1', '4', '\0', '1', '1', '5', '\0', '1', '1', '6', '\0', '1', '1', '7', '\0', '1', '1', '8', '\0', '1', '1', '9', '\0',
	 '1', '2', '0', '\0', '1', '2', '1', '\0', '1', '2', '2', '\0', '1', '2', '3', '\0', '1', '2', '4', '\0', '1', '2', '5', '\0', '1', '2', '6', '\0', '1', '2', '7', '\0', '1', '2', '8', '\0', '1', '2', '9', '\0',
	 '1', '3', '0', '\0', '1', '3', '1', '\0', '1', '3', '2', '\0', '1', '3', '3', '\0', '1', '3', '4', '\0', '1', '3', '5', '\0', '1', '3', '6', '\0', '1', '3', '7', '\0', '1', '3', '8', '\0', '1', '3', '9', '\0',
	 '1', '4', '0', '\0', '1', '4', '1', '\0', '1', '4', '2', '\0', '1', '4', '3', '\0', '1', '4', '4', '\0', '1', '4', '5', '\0', '1', '4', '6', '\0', '1', '4', '7', '\0', '1', '4', '8', '\0', '1', '4', '9', '\0',
	 '1', '5', '0', '\0', '1', '5', '1', '\0', '1', '5', '2', '\0', '1', '5', '3', '\0', '1', '5', '4', '\0', '1', '5', '5', '\0', '1', '5', '6', '\0', '1', '5', '7', '\0', '1', '5', '8', '\0', '1', '5', '9', '\0',
	 '1', '6', '0', '\0', '1', '6', '1', '\0', '1', '6', '2', '\0', '1', '6', '3', '\0', '1', '6', '4', '\0', '1', '6', '5', '\0', '1', '6', '6', '\0', '1', '6', '7', '\0', '1', '6', '8', '\0', '1', '6', '9', '\0',
	 '1', '7', '0', '\0', '1', '7', '1', '\0', '1', '7', '2', '\0', '1', '7', '3', '\0', '1', '7', '4', '\0', '1', '7', '5', '\0', '1', '7', '6', '\0', '1', '7', '7', '\0', '1', '7', '8', '\0', '1', '7', '9', '\0',
	 '1', '8', '0', '\0', '1', '8', '1', '\0', '1', '8', '2', '\0', '1', '8', '3', '\0', '1', '8', '4', '\0', '1', '8', '5', '\0', '1', '8', '6', '\0', '1', '8', '7', '\0', '1', '8', '8', '\0', '1', '8', '9', '\0',
	 '1', '9', '0', '\0', '1', '9', '1', '\0', '1', '9', '2', '\0', '1', '9', '3', '\0', '1', '9', '4', '\0', '1', '9', '5', '\0', '1', '9', '6', '\0', '1', '9', '7', '\0', '1', '9', '8', '\0', '1', '9', '9', '\0',
	 '2', '0', '0', '\0', '2', '0', '1', '\0', '2', '0', '2', '\0', '2', '0', '3', '\0', '2', '0', '4', '\0', '2', '0', '5', '\0', '2', '0', '6', '\0', '2', '0', '7', '\0', '2', '0', '8', '\0', '2', '0', '9', '\0',
	 '2', '1', '0', '\0', '2', '1', '1', '\0', '2', '1', '2', '\0', '2', '1', '3', '\0', '2', '1', '4', '\0', '2', '1', '5', '\0', '2', '1', '6', '\0', '2', '1', '7', '\0', '2', '1', '8', '\0', '2', '1', '9', '\0',
	 '2', '2', '0', '\0', '2', '2', '1', '\0', '2', '2', '2', '\0', '2', '2', '3', '\0', '2', '2', '4', '\0', '2', '2', '5', '\0', '2', '2', '6', '\0', '2', '2', '7', '\0', '2', '2', '8', '\0', '2', '2', '9', '\0',
	 '2', '3', '0', '\0', '2', '3', '1', '\0', '2', '3', '2', '\0', '2', '3', '3', '\0', '2', '3', '4', '\0', '2', '3', '5', '\0', '2', '3', '6', '\0', '2', '3', '7', '\0', '2', '3', '8', '\0', '2', '3', '9', '\0',
	 '2', '4', '0', '\0', '2', '4', '1', '\0', '2', '4', '2', '\0', '2', '4', '3', '\0', '2', '4', '4', '\0', '2', '4', '5', '\0', '2', '4', '6', '\0', '2', '4', '7', '\0', '2', '4', '8', '\0', '2', '4', '9', '\0',
	 '2', '5', '0', '\0', '2', '5', '1', '\0', '2', '5', '2', '\0', '2', '5', '3', '\0', '2', '5', '4', '\0', '2', '5', '5', '\0', '2', '5', '6', '\0', '2', '5', '7', '\0', '2', '5', '8', '\0', '2', '5', '9', '\0',
	 '2', '6', '0', '\0', '2', '6', '1', '\0', '2', '6', '2', '\0', '2', '6', '3', '\0', '2', '6', '4', '\0', '2', '6', '5', '\0', '2', '6', '6', '\0', '2', '6', '7', '\0', '2', '6', '8', '\0', '2', '6', '9', '\0',
	 '2', '7', '0', '\0', '2', '7', '1', '\0', '2', '7', '2', '\0', '2', '7', '3', '\0', '2', '7', '4', '\0', '2', '7', '5', '\0', '2', '7', '6', '\0', '2', '7', '7', '\0', '2', '7', '8', '\0', '2', '7', '9', '\0',
	 '2', '8', '0', '\0', '2', '8', '1', '\0', '2', '8', '2', '\0', '2', '8', '3', '\0', '2', '8', '4', '\0', '2', '8', '5', '\0', '2', '8', '6', '\0', '2', '8', '7', '\0', '2', '8', '8', '\0', '2', '8', '9', '\0',
	 '2', '9', '0', '\0', '2', '9', '1', '\0', '2', '9', '2', '\0', '2', '9', '3', '\0', '2', '9', '4', '\0', '2', '9', '5', '\0', '2', '9', '6', '\0', '2', '9', '7', '\0', '2', '9', '8', '\0', '2', '9', '9', '\0',
	 '3', '0', '0', '\0', '3', '0', '1', '\0', '3', '0', '2', '\0', '3', '0', '3', '\0', '3', '0', '4', '\0', '3', '0', '5', '\0', '3', '0', '6', '\0', '3', '0', '7', '\0', '3', '0', '8', '\0', '3', '0', '9', '\0',
	 '3', '1', '0', '\0', '3', '1', '1', '\0', '3', '1', '2', '\0', '3', '1', '3', '\0', '3', '1', '4', '\0', '3', '1', '5', '\0', '3', '1', '6', '\0', '3', '1', '7', '\0', '3', '1', '8', '\0', '3', '1', '9', '\0',
	 '3', '2', '0', '\0', '3', '2', '1', '\0', '3', '2', '2', '\0', '3', '2', '3', '\0', '3', '2', '4', '\0', '3', '2', '5', '\0', '3', '2', '6', '\0', '3', '2', '7', '\0', '3', '2', '8', '\0', '3', '2', '9', '\0',
	 '3', '3', '0', '\0', '3', '3', '1', '\0', '3', '3', '2', '\0', '3', '3', '3', '\0', '3', '3', '4', '\0', '3', '3', '5', '\0', '3', '3', '6', '\0', '3', '3', '7', '\0', '3', '3', '8', '\0', '3', '3', '9', '\0',
	 '3', '4', '0', '\0', '3', '4', '1', '\0', '3', '4', '2', '\0', '3', '4', '3', '\0', '3', '4', '4', '\0', '3', '4', '5', '\0', '3', '4', '6', '\0', '3', '4', '7', '\0', '3', '4', '8', '\0', '3', '4', '9', '\0',
	 '3', '5', '0', '\0', '3', '5', '1', '\0', '3', '5', '2', '\0', '3', '5', '3', '\0', '3', '5', '4', '\0', '3', '5', '5', '\0', '3', '5', '6', '\0', '3', '5', '7', '\0', '3', '5', '8', '\0', '3', '5', '9', '\0',
	 '3', '6', '0', '\0', '3', '6', '1', '\0', '3', '6', '2', '\0', '3', '6', '3', '\0', '3', '6', '4', '\0', '3', '6', '5', '\0', '3', '6', '6', '\0', '3', '6', '7', '\0', '3', '6', '8', '\0', '3', '6', '9', '\0',
	 '3', '7', '0', '\0', '3', '7', '1', '\0', '3', '7', '2', '\0', '3', '7', '3', '\0', '3', '7', '4', '\0', '3', '7', '5', '\0', '3', '7', '6', '\0', '3', '7', '7', '\0', '3', '7', '8', '\0', '3', '7', '9', '\0',
	 '3', '8', '0', '\0', '3', '8', '1', '\0', '3', '8', '2', '\0', '3', '8', '3', '\0', '3', '8', '4', '\0', '3', '8', '5', '\0', '3', '8', '6', '\0', '3', '8', '7', '\0', '3', '8', '8', '\0', '3', '8', '9', '\0',
	 '3', '9', '0', '\0', '3', '9', '1', '\0', '3', '9', '2', '\0', '3', '9', '3', '\0', '3', '9', '4', '\0', '3', '9', '5', '\0', '3', '9', '6', '\0', '3', '9', '7', '\0', '3', '9', '8', '\0', '3', '9', '9', '\0',
	 '4', '0', '0', '\0', '4', '0', '1', '\0', '4', '0', '2', '\0', '4', '0', '3', '\0', '4', '0', '4', '\0', '4', '0', '5', '\0', '4', '0', '6', '\0', '4', '0', '7', '\0', '4', '0', '8', '\0', '4', '0', '9', '\0',
	 '4', '1', '0', '\0', '4', '1', '1', '\0', '4', '1', '2', '\0', '4', '1', '3', '\0', '4', '1', '4', '\0', '4', '1', '5', '\0', '4', '1', '6', '\0', '4', '1', '7', '\0', '4', '1', '8', '\0', '4', '1', '9', '\0',
	 '4', '2', '0', '\0', '4', '2', '1', '\0', '4', '2', '2', '\0', '4', '2', '3', '\0', '4', '2', '4', '\0', '4', '2', '5', '\0', '4', '2', '6', '\0', '4', '2', '7', '\0', '4', '2', '8', '\0', '4', '2', '9', '\0',
	 '4', '3', '0', '\0', '4', '3', '1', '\0', '4', '3', '2', '\0', '4', '3', '3', '\0', '4', '3', '4', '\0', '4', '3', '5', '\0', '4', '3', '6', '\0', '4', '3', '7', '\0', '4', '3', '8', '\0', '4', '3', '9', '\0',
	 '4', '4', '0', '\0', '4', '4', '1', '\0', '4', '4', '2', '\0', '4', '4', '3', '\0', '4', '4', '4', '\0', '4', '4', '5', '\0', '4', '4', '6', '\0', '4', '4', '7', '\0', '4', '4', '8', '\0', '4', '4', '9', '\0',
	 '4', '5', '0', '\0', '4', '5', '1', '\0', '4', '5', '2', '\0', '4', '5', '3', '\0', '4', '5', '4', '\0', '4', '5', '5', '\0', '4', '5', '6', '\0', '4', '5', '7', '\0', '4', '5', '8', '\0', '4', '5', '9', '\0',
	 '4', '6', '0', '\0', '4', '6', '1', '\0', '4', '6', '2', '\0', '4', '6', '3', '\0', '4', '6', '4', '\0', '4', '6', '5', '\0', '4', '6', '6', '\0', '4', '6', '7', '\0', '4', '6', '8', '\0', '4', '6', '9', '\0',
	 '4', '7', '0', '\0', '4', '7', '1', '\0', '4', '7', '2', '\0', '4', '7', '3', '\0', '4', '7', '4', '\0', '4', '7', '5', '\0', '4', '7', '6', '\0', '4', '7', '7', '\0', '4', '7', '8', '\0', '4', '7', '9', '\0',
	 '4', '8', '0', '\0', '4', '8', '1', '\0', '4', '8', '2', '\0', '4', '8', '3', '\0', '4', '8', '4', '\0', '4', '8', '5', '\0', '4', '8', '6', '\0', '4', '8', '7', '\0', '4', '8', '8', '\0', '4', '8', '9', '\0',
	 '4', '9', '0', '\0', '4', '9', '1', '\0', '4', '9', '2', '\0', '4', '9', '3', '\0', '4', '9', '4', '\0', '4', '9', '5', '\0', '4', '9', '6', '\0', '4', '9', '7', '\0', '4', '9', '8', '\0', '4', '9', '9', '\0',
	 '5', '0', '0', '\0', '5', '0', '1', '\0', '5', '0', '2', '\0', '5', '0', '3', '\0', '5', '0', '4', '\0', '5', '0', '5', '\0', '5', '0', '6', '\0', '5', '0', '7', '\0', '5', '0', '8', '\0', '5', '0', '9', '\0',
	 '5', '1', '0', '\0', '5', '1', '1', '\0', '5', '1', '2', '\0', '5', '1', '3', '\0', '5', '1', '4', '\0', '5', '1', '5', '\0', '5', '1', '6', '\0', '5', '1', '7', '\0', '5', '1', '8', '\0', '5', '1', '9', '\0',
	 '5', '2', '0', '\0', '5', '2', '1', '\0', '5', '2', '2', '\0', '5', '2', '3', '\0', '5', '2', '4', '\0', '5', '2', '5', '\0', '5', '2', '6', '\0', '5', '2', '7', '\0', '5', '2', '8', '\0', '5', '2', '9', '\0',
	 '5', '3', '0', '\0', '5', '3', '1', '\0', '5', '3', '2', '\0', '5', '3', '3', '\0', '5', '3', '4', '\0', '5', '3', '5', '\0', '5', '3', '6', '\0', '5', '3', '7', '\0', '5', '3', '8', '\0', '5', '3', '9', '\0',
	 '5', '4', '0', '\0', '5', '4', '1', '\0', '5', '4', '2', '\0', '5', '4', '3', '\0', '5', '4', '4', '\0', '5', '4', '5', '\0', '5', '4', '6', '\0', '5', '4', '7', '\0', '5', '4', '8', '\0', '5', '4', '9', '\0',
	 '5', '5', '0', '\0', '5', '5', '1', '\0', '5', '5', '2', '\0', '5', '5', '3', '\0', '5', '5', '4', '\0', '5', '5', '5', '\0', '5', '5', '6', '\0', '5', '5', '7', '\0', '5', '5', '8', '\0', '5', '5', '9', '\0',
	 '5', '6', '0', '\0', '5', '6', '1', '\0', '5', '6', '2', '\0', '5', '6', '3', '\0', '5', '6', '4', '\0', '5', '6', '5', '\0', '5', '6', '6', '\0', '5', '6', '7', '\0', '5', '6', '8', '\0', '5', '6', '9', '\0',
	 '5', '7', '0', '\0', '5', '7', '1', '\0', '5', '7', '2', '\0', '5', '7', '3', '\0', '5', '7', '4', '\0', '5', '7', '5', '\0', '5', '7', '6', '\0', '5', '7', '7', '\0', '5', '7', '8', '\0', '5', '7', '9', '\0',
	 '5', '8', '0', '\0', '5', '8', '1', '\0', '5', '8', '2', '\0', '5', '8', '3', '\0', '5', '8', '4', '\0', '5', '8', '5', '\0', '5', '8', '6', '\0', '5', '8', '7', '\0', '5', '8', '8', '\0', '5', '8', '9', '\0',
	 '5', '9', '0', '\0', '5', '9', '1', '\0', '5', '9', '2', '\0', '5', '9', '3', '\0', '5', '9', '4', '\0', '5', '9', '5', '\0', '5', '9', '6', '\0', '5', '9', '7', '\0', '5', '9', '8', '\0', '5', '9', '9', '\0',
	 '6', '0', '0', '\0', '6', '0', '1', '\0', '6', '0', '2', '\0', '6', '0', '3', '\0', '6', '0', '4', '\0', '6', '0', '5', '\0', '6', '0', '6', '\0', '6', '0', '7', '\0', '6', '0', '8', '\0', '6', '0', '9', '\0',
	 '6', '1', '0', '\0', '6', '1', '1', '\0', '6', '1', '2', '\0', '6', '1', '3', '\0', '6', '1', '4', '\0', '6', '1', '5', '\0', '6', '1', '6', '\0', '6', '1', '7', '\0', '6', '1', '8', '\0', '6', '1', '9', '\0',
	 '6', '2', '0', '\0', '6', '2', '1', '\0', '6', '2', '2', '\0', '6', '2', '3', '\0', '6', '2', '4', '\0', '6', '2', '5', '\0', '6', '2', '6', '\0', '6', '2', '7', '\0', '6', '2', '8', '\0', '6', '2', '9', '\0',
	 '6', '3', '0', '\0', '6', '3', '1', '\0', '6', '3', '2', '\0', '6', '3', '3', '\0', '6', '3', '4', '\0', '6', '3', '5', '\0', '6', '3', '6', '\0', '6', '3', '7', '\0', '6', '3', '8', '\0', '6', '3', '9', '\0',
	 '6', '4', '0', '\0', '6', '4', '1', '\0', '6', '4', '2', '\0', '6', '4', '3', '\0', '6', '4', '4', '\0', '6', '4', '5', '\0', '6', '4', '6', '\0', '6', '4', '7', '\0', '6', '4', '8', '\0', '6', '4', '9', '\0',
	 '6', '5', '0', '\0', '6', '5', '1', '\0', '6', '5', '2', '\0', '6', '5', '3', '\0', '6', '5', '4', '\0', '6', '5', '5', '\0', '6', '5', '6', '\0', '6', '5', '7', '\0', '6', '5', '8', '\0', '6', '5', '9', '\0',
	 '6', '6', '0', '\0', '6', '6', '1', '\0', '6', '6', '2', '\0', '6', '6', '3', '\0', '6', '6', '4', '\0', '6', '6', '5', '\0', '6', '6', '6', '\0', '6', '6', '7', '\0', '6', '6', '8', '\0', '6', '6', '9', '\0',
	 '6', '7', '0', '\0', '6', '7', '1', '\0', '6', '7', '2', '\0', '6', '7', '3', '\0', '6', '7', '4', '\0', '6', '7', '5', '\0', '6', '7', '6', '\0', '6', '7', '7', '\0', '6', '7', '8', '\0', '6', '7', '9', '\0',
	 '6', '8', '0', '\0', '6', '8', '1', '\0', '6', '8', '2', '\0', '6', '8', '3', '\0', '6', '8', '4', '\0', '6', '8', '5', '\0', '6', '8', '6', '\0', '6', '8', '7', '\0', '6', '8', '8', '\0', '6', '8', '9', '\0',
	 '6', '9', '0', '\0', '6', '9', '1', '\0', '6', '9', '2', '\0', '6', '9', '3', '\0', '6', '9', '4', '\0', '6', '9', '5', '\0', '6', '9', '6', '\0', '6', '9', '7', '\0', '6', '9', '8', '\0', '6', '9', '9', '\0',
	 '7', '0', '0', '\0', '7', '0', '1', '\0', '7', '0', '2', '\0', '7', '0', '3', '\0', '7', '0', '4', '\0', '7', '0', '5', '\0', '7', '0', '6', '\0', '7', '0', '7', '\0', '7', '0', '8', '\0', '7', '0', '9', '\0',
	 '7', '1', '0', '\0', '7', '1', '1', '\0', '7', '1', '2', '\0', '7', '1', '3', '\0', '7', '1', '4', '\0', '7', '1', '5', '\0', '7', '1', '6', '\0', '7', '1', '7', '\0', '7', '1', '8', '\0', '7', '1', '9', '\0',
	 '7', '2', '0', '\0', '7', '2', '1', '\0', '7', '2', '2', '\0', '7', '2', '3', '\0', '7', '2', '4', '\0', '7', '2', '5', '\0', '7', '2', '6', '\0', '7', '2', '7', '\0', '7', '2', '8', '\0', '7', '2', '9', '\0',
	 '7', '3', '0', '\0', '7', '3', '1', '\0', '7', '3', '2', '\0', '7', '3', '3', '\0', '7', '3', '4', '\0', '7', '3', '5', '\0', '7', '3', '6', '\0', '7', '3', '7', '\0', '7', '3', '8', '\0', '7', '3', '9', '\0',
	 '7', '4', '0', '\0', '7', '4', '1', '\0', '7', '4', '2', '\0', '7', '4', '3', '\0', '7', '4', '4', '\0', '7', '4', '5', '\0', '7', '4', '6', '\0', '7', '4', '7', '\0', '7', '4', '8', '\0', '7', '4', '9', '\0',
	 '7', '5', '0', '\0', '7', '5', '1', '\0', '7', '5', '2', '\0', '7', '5', '3', '\0', '7', '5', '4', '\0', '7', '5', '5', '\0', '7', '5', '6', '\0', '7', '5', '7', '\0', '7', '5', '8', '\0', '7', '5', '9', '\0',
	 '7', '6', '0', '\0', '7', '6', '1', '\0', '7', '6', '2', '\0', '7', '6', '3', '\0', '7', '6', '4', '\0', '7', '6', '5', '\0', '7', '6', '6', '\0', '7', '6', '7', '\0', '7', '6', '8', '\0', '7', '6', '9', '\0',
	 '7', '7', '0', '\0', '7', '7', '1', '\0', '7', '7', '2', '\0', '7', '7', '3', '\0', '7', '7', '4', '\0', '7', '7', '5', '\0', '7', '7', '6', '\0', '7', '7', '7', '\0', '7', '7', '8', '\0', '7', '7', '9', '\0',
	 '7', '8', '0', '\0', '7', '8', '1', '\0', '7', '8', '2', '\0', '7', '8', '3', '\0', '7', '8', '4', '\0', '7', '8', '5', '\0', '7', '8', '6', '\0', '7', '8', '7', '\0', '7', '8', '8', '\0', '7', '8', '9', '\0',
	 '7', '9', '0', '\0', '7', '9', '1', '\0', '7', '9', '2', '\0', '7', '9', '3', '\0', '7', '9', '4', '\0', '7', '9', '5', '\0', '7', '9', '6', '\0', '7', '9', '7', '\0', '7', '9', '8', '\0', '7', '9', '9', '\0',
	 '8', '0', '0', '\0', '8', '0', '1', '\0', '8', '0', '2', '\0', '8', '0', '3', '\0', '8', '0', '4', '\0', '8', '0', '5', '\0', '8', '0', '6', '\0', '8', '0', '7', '\0', '8', '0', '8', '\0', '8', '0', '9', '\0',
	 '8', '1', '0', '\0', '8', '1', '1', '\0', '8', '1', '2', '\0', '8', '1', '3', '\0', '8', '1', '4', '\0', '8', '1', '5', '\0', '8', '1', '6', '\0', '8', '1', '7', '\0', '8', '1', '8', '\0', '8', '1', '9', '\0',
	 '8', '2', '0', '\0', '8', '2', '1', '\0', '8', '2', '2', '\0', '8', '2', '3', '\0', '8', '2', '4', '\0', '8', '2', '5', '\0', '8', '2', '6', '\0', '8', '2', '7', '\0', '8', '2', '8', '\0', '8', '2', '9', '\0',
	 '8', '3', '0', '\0', '8', '3', '1', '\0', '8', '3', '2', '\0', '8', '3', '3', '\0', '8', '3', '4', '\0', '8', '3', '5', '\0', '8', '3', '6', '\0', '8', '3', '7', '\0', '8', '3', '8', '\0', '8', '3', '9', '\0',
	 '8', '4', '0', '\0', '8', '4', '1', '\0', '8', '4', '2', '\0', '8', '4', '3', '\0', '8', '4', '4', '\0', '8', '4', '5', '\0', '8', '4', '6', '\0', '8', '4', '7', '\0', '8', '4', '8', '\0', '8', '4', '9', '\0',
	 '8', '5', '0', '\0', '8', '5', '1', '\0', '8', '5', '2', '\0', '8', '5', '3', '\0', '8', '5', '4', '\0', '8', '5', '5', '\0', '8', '5', '6', '\0', '8', '5', '7', '\0', '8', '5', '8', '\0', '8', '5', '9', '\0',
	 '8', '6', '0', '\0', '8', '6', '1', '\0', '8', '6', '2', '\0', '8', '6', '3', '\0', '8', '6', '4', '\0', '8', '6', '5', '\0', '8', '6', '6', '\0', '8', '6', '7', '\0', '8', '6', '8', '\0', '8', '6', '9', '\0',
	 '8', '7', '0', '\0', '8', '7', '1', '\0', '8', '7', '2', '\0', '8', '7', '3', '\0', '8', '7', '4', '\0', '8', '7', '5', '\0', '8', '7', '6', '\0', '8', '7', '7', '\0', '8', '7', '8', '\0', '8', '7', '9', '\0',
	 '8', '8', '0', '\0', '8', '8', '1', '\0', '8', '8', '2', '\0', '8', '8', '3', '\0', '8', '8', '4', '\0', '8', '8', '5', '\0', '8', '8', '6', '\0', '8', '8', '7', '\0', '8', '8', '8', '\0', '8', '8', '9', '\0',
	 '8', '9', '0', '\0', '8', '9', '1', '\0', '8', '9', '2', '\0', '8', '9', '3', '\0', '8', '9', '4', '\0', '8', '9', '5', '\0', '8', '9', '6', '\0', '8', '9', '7', '\0', '8', '9', '8', '\0', '8', '9', '9', '\0',
	 '9', '0', '0', '\0', '9', '0', '1', '\0', '9', '0', '2', '\0', '9', '0', '3', '\0', '9', '0', '4', '\0', '9', '0', '5', '\0', '9', '0', '6', '\0', '9', '0', '7', '\0', '9', '0', '8', '\0', '9', '0', '9', '\0',
	 '9', '1', '0', '\0', '9', '1', '1', '\0', '9', '1', '2', '\0', '9', '1', '3', '\0', '9', '1', '4', '\0', '9', '1', '5', '\0', '9', '1', '6', '\0', '9', '1', '7', '\0', '9', '1', '8', '\0', '9', '1', '9', '\0',
	 '9', '2', '0', '\0', '9', '2', '1', '\0', '9', '2', '2', '\0', '9', '2', '3', '\0', '9', '2', '4', '\0', '9', '2', '5', '\0', '9', '2', '6', '\0', '9', '2', '7', '\0', '9', '2', '8', '\0', '9', '2', '9', '\0',
	 '9', '3', '0', '\0', '9', '3', '1', '\0', '9', '3', '2', '\0', '9', '3', '3', '\0', '9', '3', '4', '\0', '9', '3', '5', '\0', '9', '3', '6', '\0', '9', '3', '7', '\0', '9', '3', '8', '\0', '9', '3', '9', '\0',
	 '9', '4', '0', '\0', '9', '4', '1', '\0', '9', '4', '2', '\0', '9', '4', '3', '\0', '9', '4', '4', '\0', '9', '4', '5', '\0', '9', '4', '6', '\0', '9', '4', '7', '\0', '9', '4', '8', '\0', '9', '4', '9', '\0',
	 '9', '5', '0', '\0', '9', '5', '1', '\0', '9', '5', '2', '\0', '9', '5', '3', '\0', '9', '5', '4', '\0', '9', '5', '5', '\0', '9', '5', '6', '\0', '9', '5', '7', '\0', '9', '5', '8', '\0', '9', '5', '9', '\0',
	 '9', '6', '0', '\0', '9', '6', '1', '\0', '9', '6', '2', '\0', '9', '6', '3', '\0', '9', '6', '4', '\0', '9', '6', '5', '\0', '9', '6', '6', '\0', '9', '6', '7', '\0', '9', '6', '8', '\0', '9', '6', '9', '\0',
	 '9', '7', '0', '\0', '9', '7', '1', '\0', '9', '7', '2', '\0', '9', '7', '3', '\0', '9', '7', '4', '\0', '9', '7', '5', '\0', '9', '7', '6', '\0', '9', '7', '7', '\0', '9', '7', '8', '\0', '9', '7', '9', '\0',
	 '9', '8', '0', '\0', '9', '8', '1', '\0', '9', '8', '2', '\0', '9', '8', '3', '\0', '9', '8', '4', '\0', '9', '8', '5', '\0', '9', '8', '6', '\0', '9', '8', '7', '\0', '9', '8', '8', '\0', '9', '8', '9', '\0',
	 '9', '9', '0', '\0', '9', '9', '1', '\0', '9', '9', '2', '\0', '9', '9', '3', '\0', '9', '9', '4', '\0', '9', '9', '5', '\0', '9', '9', '6', '\0', '9', '9', '7', '\0', '9', '9', '8', '\0', '9', '9', '9', '\0',
};
static_assert(alignof(DecimalDigits_000_999) == 4);

static const char DecimalDigits_00_99[] alignas(2) = {
	 '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
	 '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
	 '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
	 '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
	 '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
	 '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
	 '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
	 '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
	 '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
	 '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9',
};
static_assert(alignof(DecimalDigits_00_99) == 2);

EJ_ALWAYS_INLINE unsigned small_floor_log10_2_e(unsigned exponent) noexcept {
	assert(exponent >= 0 && exponent <= 680);
	return exponent * 1233u / 4096u;
}

unsigned uint32_find_number_of_digits(uint32_t value) noexcept {
	if (value > 0) {
		//Calculate the number of decimal digits by the floor of log10 of the closest power of 2
#ifdef __LZCNT__
		auto n = small_floor_log10_2_e(32u - _lzcnt_u32(value));
#else
		auto n = small_floor_log10_2_e(bsr32_nz(value | 1u) + 1u);
#endif
		n += (value >= static_cast<uint32_t>(Uint64PowersOf10[n]) ? 1u : 0u);
		return n;
	} else {
		return 1;
	}
}

char *uint32_to_string_no_nul(char *s, uint32_t value) noexcept {
	//Calculate the number of decimal digits by the floor of log10 of the closest power of 2
#ifdef __LZCNT__
	auto n = small_floor_log10_2_e(32u - _lzcnt_u32(value));
#else
	auto n = small_floor_log10_2_e(bsr32_nz(value | 1u) + 1u);
#endif
	n += (value >= static_cast<uint32_t>(Uint64PowersOf10[n]) ? 1u : 0u);
	auto *e = s + n;
	uint32_t digits;

	//Write 3 digits at a time to the string
	switch (n) {
	case 9:
		digits = value / 1000000u;
		memcpy(e - 9, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 1000000u;
	case 6:
		digits = value / 1000u;
		memcpy(e - 6, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 1000u;
	case 3:
		memcpy(e - 3, DecimalDigits_000_999 + value * 4, 4);
	default:
		return e;

	case 8:
		digits = value / 100000u;
		memcpy(e - 8, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 100000u;
	case 5:
		digits = value / 100u;
		memcpy(e - 5, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 100u;
	case 2:
		memcpy(e - 2, DecimalDigits_00_99 + value * 2, 2);
		return e;

	case 10:
		digits = value / 10000000u;
		memcpy(e - 10, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 10000000u;
	case 7:
		digits = value / 10000u;
		memcpy(e - 7, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 10000u;
	case 4:
		digits = value / 10u;
		memcpy(e - 4, DecimalDigits_000_999 + digits * 4, 4);
		value -= digits * 10u;
	case 1:
		e[-1] = '0' + value;
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

unsigned uint64_find_number_of_digits(uint64_t value) noexcept {
	if (value > 0) {
		//Calculate the number of decimal digits by the floor of log10 of the closest power of 2
#ifdef __LZCNT__
		auto n = small_floor_log10_2_e(64u - static_cast<unsigned>(_lzcnt_u64(value)));
#else
		auto n = small_floor_log10_2_e(static_cast<unsigned>(bsr64_nz(value | 1u)) + 1u);
#endif
		n += (value >= Uint64PowersOf10[n] ? 1u : 0u);
		return n;
	} else {
		return 1;
	}
}

char *uint64_to_string_no_nul(char *s, uint64_t value) noexcept {
	//Calculate the number of decimal digits by the floor of log10 of the closest power of 2
#ifdef __LZCNT__
	auto n = small_floor_log10_2_e(64u - static_cast<uint32_t>(_lzcnt_u64(value)));
#else
	auto n = small_floor_log10_2_e(static_cast<uint32_t>(bsr64_nz(value | 1u)) + 1u);
#endif
	n += (value >= Uint64PowersOf10[n] ? 1u : 0u);
	auto *e = s + n;
	uint32_t digits, value32;

	//Write 3 digits at a time to the string
	value32 = static_cast<uint32_t>(value);
	switch (n) {
	case 18:
		digits = value / UINT64_C(1000000000000000);
		memcpy(e - 18, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(1000000000000000);
	case 15:
		digits = value / UINT64_C(1000000000000);
		memcpy(e - 15, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(1000000000000);
	case 12:
		digits = value / 1000000000u;
		memcpy(e - 12, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * 1000000000u;
		value32 = static_cast<uint32_t>(value);
	case 9:
		digits = value32 / 1000000u;
		memcpy(e - 9, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 1000000u;
	case 6:
		digits = value32 / 1000u;
		memcpy(e - 6, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 1000u;
	case 3:
		memcpy(e - 3, DecimalDigits_000_999 + value32 * 4, 4);
	default:
		return e;

	case 20:
		digits = value / UINT64_C(100000000000000000);
		memcpy(e - 20, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(100000000000000000);
	case 17:
		digits = value / UINT64_C(100000000000000);
		memcpy(e - 17, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(100000000000000);
	case 14:
		digits = value / UINT64_C(100000000000);
		memcpy(e - 14, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(100000000000);
	case 11:
		digits = value / 100000000u;
		memcpy(e - 11, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * 100000000u;
		value32 = static_cast<uint32_t>(value);
	case 8:
		digits = value32 / 100000u;
		memcpy(e - 8, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 100000u;
	case 5:
		digits = value32 / 100u;
		memcpy(e - 5, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 100u;
	case 2:
		memcpy(e - 2, DecimalDigits_00_99 + value32 * 2, 2);
		return e;

	case 19:
		digits = value / UINT64_C(10000000000000000);
		memcpy(e - 19, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(10000000000000000);
	case 16:
		digits = value / UINT64_C(10000000000000);
		memcpy(e - 16, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(10000000000000);
	case 13:
		digits = value / UINT64_C(10000000000);
		memcpy(e - 13, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * UINT64_C(10000000000);
	case 10:
		digits = value / 10000000u;
		memcpy(e - 10, DecimalDigits_000_999 + digits * 4, 4);
		value -= static_cast<uint64_t>(digits) * 10000000u;
		value32 = static_cast<uint32_t>(value);
	case 7:
		digits = value32 / 10000u;
		memcpy(e - 7, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 10000u;
	case 4:
		digits = value32 / 10u;
		memcpy(e - 4, DecimalDigits_000_999 + digits * 4, 4);
		value32 -= digits * 10u;
	case 1:
		e[-1] = '0' + value32;
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

static const char HexDigits_0_f[] = {
	 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

static const char HexDigits_00_ff[] alignas(2) = {
	 '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9', '0', 'a', '0', 'b', '0', 'c', '0', 'd', '0', 'e', '0', 'f',
	 '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9', '1', 'a', '1', 'b', '1', 'c', '1', 'd', '1', 'e', '1', 'f',
	 '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9', '2', 'a', '2', 'b', '2', 'c', '2', 'd', '2', 'e', '2', 'f',
	 '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9', '3', 'a', '3', 'b', '3', 'c', '3', 'd', '3', 'e', '3', 'f',
	 '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9', '4', 'a', '4', 'b', '4', 'c', '4', 'd', '4', 'e', '4', 'f',
	 '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9', '5', 'a', '5', 'b', '5', 'c', '5', 'd', '5', 'e', '5', 'f',
	 '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9', '6', 'a', '6', 'b', '6', 'c', '6', 'd', '6', 'e', '6', 'f',
	 '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9', '7', 'a', '7', 'b', '7', 'c', '7', 'd', '7', 'e', '7', 'f',
	 '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9', '8', 'a', '8', 'b', '8', 'c', '8', 'd', '8', 'e', '8', 'f',
	 '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9', '9', 'a', '9', 'b', '9', 'c', '9', 'd', '9', 'e', '9', 'f',
	 'a', '0', 'a', '1', 'a', '2', 'a', '3', 'a', '4', 'a', '5', 'a', '6', 'a', '7', 'a', '8', 'a', '9', 'a', 'a', 'a', 'b', 'a', 'c', 'a', 'd', 'a', 'e', 'a', 'f',
	 'b', '0', 'b', '1', 'b', '2', 'b', '3', 'b', '4', 'b', '5', 'b', '6', 'b', '7', 'b', '8', 'b', '9', 'b', 'a', 'b', 'b', 'b', 'c', 'b', 'd', 'b', 'e', 'b', 'f',
	 'c', '0', 'c', '1', 'c', '2', 'c', '3', 'c', '4', 'c', '5', 'c', '6', 'c', '7', 'c', '8', 'c', '9', 'c', 'a', 'c', 'b', 'c', 'c', 'c', 'd', 'c', 'e', 'c', 'f',
	 'd', '0', 'd', '1', 'd', '2', 'd', '3', 'd', '4', 'd', '5', 'd', '6', 'd', '7', 'd', '8', 'd', '9', 'd', 'a', 'd', 'b', 'd', 'c', 'd', 'd', 'd', 'e', 'd', 'f',
	 'e', '0', 'e', '1', 'e', '2', 'e', '3', 'e', '4', 'e', '5', 'e', '6', 'e', '7', 'e', '8', 'e', '9', 'e', 'a', 'e', 'b', 'e', 'c', 'e', 'd', 'e', 'e', 'e', 'f',
	 'f', '0', 'f', '1', 'f', '2', 'f', '3', 'f', '4', 'f', '5', 'f', '6', 'f', '7', 'f', '8', 'f', '9', 'f', 'a', 'f', 'b', 'f', 'c', 'f', 'd', 'f', 'e', 'f', 'f',
};
static_assert(alignof(HexDigits_00_ff) == 2);

char *uint32_to_hex_string_no_nul(char *s, uint32_t value) noexcept {
	//Calculate the number of hex digits
	auto n = bsr32_nz(value | 1u) / 4u + 1u;
	auto *e = s + n;
	uint32_t digits;

	//Write 2 digits at a time to the string
	switch (n) {
	case 8:
		digits = value >> 24;
		memcpy(e - 8, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFFFFFFu;
	case 6:
		digits = value >> 16;
		memcpy(e - 6, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFFFFu;
	case 4:
		digits = value >> 8;
		memcpy(e - 4, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFFu;
	case 2:
		memcpy(e - 2, HexDigits_00_ff + value * 2, 2);
	default:
		return e;

	case 7:
		digits = value >> 20;
		memcpy(e - 7, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFFFFFu;
	case 5:
		digits = value >> 12;
		memcpy(e - 5, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFFFu;
	case 3:
		digits = value >> 4;
		memcpy(e - 3, HexDigits_00_ff + digits * 2, 2);
		value &= 0xFu;
	case 1:
		e[-1] = HexDigits_0_f[value];
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

char *uint64_to_hex_string_no_nul(char *s, uint64_t value) noexcept {
	//Calculate the number of hex digits
	auto n = static_cast<unsigned>(bsr64_nz(value | 1u)) / 4u + 1u;
	auto *e = s + n;
	uint32_t digits, value32;

	//Write 2 digits at a time to the string
	value32 = static_cast<uint32_t>(value);
	switch (n) {
	case 16:
		digits = value >> 56;
		memcpy(e - 16, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFFFF);
	case 14:
		digits = value >> 48;
		memcpy(e - 14, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFF);
	case 12:
		digits = value >> 40;
		memcpy(e - 12, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFF);
	case 10:
		digits = value >> 32;
		memcpy(e - 10, HexDigits_00_ff + digits * 2, 2);
		value32 = static_cast<uint32_t>(value);
	case 8:
		digits = value32 >> 24;
		memcpy(e - 8, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFFFFFFu;
	case 6:
		digits = value32 >> 16;
		memcpy(e - 6, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFFFFu;
	case 4:
		digits = value32 >> 8;
		memcpy(e - 4, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFFu;
	case 2:
		memcpy(e - 2, HexDigits_00_ff + value32 * 2, 2);
	default:
		return e;

	case 15:
		digits = value >> 52;
		memcpy(e - 15, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFFF);
	case 13:
		digits = value >> 44;
		memcpy(e - 13, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFF);
	case 11:
		digits = value >> 36;
		memcpy(e - 11, HexDigits_00_ff + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFF);
	case 9:
		digits = value >> 28;
		memcpy(e - 9, HexDigits_00_ff + digits * 2, 2);
		value32 = static_cast<uint32_t>(value) & 0xFFFFFFFu;
	case 7:
		digits = value32 >> 20;
		memcpy(e - 7, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFFFFFu;
	case 5:
		digits = value32 >> 12;
		memcpy(e - 5, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFFFu;
	case 3:
		digits = value32 >> 4;
		memcpy(e - 3, HexDigits_00_ff + digits * 2, 2);
		value32 &= 0xFu;
	case 1:
		e[-1] = HexDigits_0_f[value32];
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

static const char HexDigits_0_F[] = {
	 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const char HexDigits_00_FF[] alignas(2) = {
	 '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9', '0', 'A', '0', 'B', '0', 'C', '0', 'D', '0', 'E', '0', 'F',
	 '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9', '1', 'A', '1', 'B', '1', 'C', '1', 'D', '1', 'E', '1', 'F',
	 '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9', '2', 'A', '2', 'B', '2', 'C', '2', 'D', '2', 'E', '2', 'F',
	 '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9', '3', 'A', '3', 'B', '3', 'C', '3', 'D', '3', 'E', '3', 'F',
	 '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9', '4', 'A', '4', 'B', '4', 'C', '4', 'D', '4', 'E', '4', 'F',
	 '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9', '5', 'A', '5', 'B', '5', 'C', '5', 'D', '5', 'E', '5', 'F',
	 '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9', '6', 'A', '6', 'B', '6', 'C', '6', 'D', '6', 'E', '6', 'F',
	 '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9', '7', 'A', '7', 'B', '7', 'C', '7', 'D', '7', 'E', '7', 'F',
	 '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9', '8', 'A', '8', 'B', '8', 'C', '8', 'D', '8', 'E', '8', 'F',
	 '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9', '9', 'A', '9', 'B', '9', 'C', '9', 'D', '9', 'E', '9', 'F',
	 'A', '0', 'A', '1', 'A', '2', 'A', '3', 'A', '4', 'A', '5', 'A', '6', 'A', '7', 'A', '8', 'A', '9', 'A', 'A', 'A', 'B', 'A', 'C', 'A', 'D', 'A', 'E', 'A', 'F',
	 'B', '0', 'B', '1', 'B', '2', 'B', '3', 'B', '4', 'B', '5', 'B', '6', 'B', '7', 'B', '8', 'B', '9', 'B', 'A', 'B', 'B', 'B', 'C', 'B', 'D', 'B', 'E', 'B', 'F',
	 'C', '0', 'C', '1', 'C', '2', 'C', '3', 'C', '4', 'C', '5', 'C', '6', 'C', '7', 'C', '8', 'C', '9', 'C', 'A', 'C', 'B', 'C', 'C', 'C', 'D', 'C', 'E', 'C', 'F',
	 'D', '0', 'D', '1', 'D', '2', 'D', '3', 'D', '4', 'D', '5', 'D', '6', 'D', '7', 'D', '8', 'D', '9', 'D', 'A', 'D', 'B', 'D', 'C', 'D', 'D', 'D', 'E', 'D', 'F',
	 'E', '0', 'E', '1', 'E', '2', 'E', '3', 'E', '4', 'E', '5', 'E', '6', 'E', '7', 'E', '8', 'E', '9', 'E', 'A', 'E', 'B', 'E', 'C', 'E', 'D', 'E', 'E', 'E', 'F',
	 'F', '0', 'F', '1', 'F', '2', 'F', '3', 'F', '4', 'F', '5', 'F', '6', 'F', '7', 'F', '8', 'F', '9', 'F', 'A', 'F', 'B', 'F', 'C', 'F', 'D', 'F', 'E', 'F', 'F',
};
static_assert(alignof(HexDigits_00_FF) == 2);

char *uint32_to_uhex_string_no_nul(char *s, uint32_t value) noexcept {
	//Calculate the number of hex digits
	auto n = bsr32_nz(value | 1u) / 4u + 1u;
	auto *e = s + n;
	uint32_t digits;

	//Write 2 digits at a time to the string
	switch (n) {
	case 8:
		digits = value >> 24;
		memcpy(e - 8, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFFFFFFu;
	case 6:
		digits = value >> 16;
		memcpy(e - 6, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFFFFu;
	case 4:
		digits = value >> 8;
		memcpy(e - 4, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFFu;
	case 2:
		memcpy(e - 2, HexDigits_00_FF + value * 2, 2);
	default:
		return e;

	case 7:
		digits = value >> 20;
		memcpy(e - 7, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFFFFFu;
	case 5:
		digits = value >> 12;
		memcpy(e - 5, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFFFu;
	case 3:
		digits = value >> 4;
		memcpy(e - 3, HexDigits_00_FF + digits * 2, 2);
		value &= 0xFu;
	case 1:
		e[-1] = HexDigits_0_F[value];
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

char *uint64_to_uhex_string_no_nul(char *s, uint64_t value) noexcept {
	//Calculate the number of hex digits
	auto n = static_cast<unsigned>(bsr64_nz(value | 1u)) / 4u + 1u;
	auto *e = s + n;
	uint32_t digits, value32;

	//Write 2 digits at a time to the string
	value32 = static_cast<uint32_t>(value);
	switch (n) {
	case 16:
		digits = value >> 56;
		memcpy(e - 16, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFFFF);
	case 14:
		digits = value >> 48;
		memcpy(e - 14, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFF);
	case 12:
		digits = value >> 40;
		memcpy(e - 12, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFF);
	case 10:
		digits = value >> 32;
		memcpy(e - 10, HexDigits_00_FF + digits * 2, 2);
		value32 = static_cast<uint32_t>(value);
	case 8:
		digits = value32 >> 24;
		memcpy(e - 8, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFFFFFFu;
	case 6:
		digits = value32 >> 16;
		memcpy(e - 6, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFFFFu;
	case 4:
		digits = value32 >> 8;
		memcpy(e - 4, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFFu;
	case 2:
		memcpy(e - 2, HexDigits_00_FF + value32 * 2, 2);
	default:
		return e;

	case 15:
		digits = value >> 52;
		memcpy(e - 15, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFFFF);
	case 13:
		digits = value >> 44;
		memcpy(e - 13, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFFFF);
	case 11:
		digits = value >> 36;
		memcpy(e - 11, HexDigits_00_FF + digits * 2, 2);
		value &= UINT64_C(0xFFFFFFFFF);
	case 9:
		digits = value >> 28;
		memcpy(e - 9, HexDigits_00_FF + digits * 2, 2);
		value32 = static_cast<uint32_t>(value) & 0xFFFFFFFu;
	case 7:
		digits = value32 >> 20;
		memcpy(e - 7, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFFFFFu;
	case 5:
		digits = value32 >> 12;
		memcpy(e - 5, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFFFu;
	case 3:
		digits = value32 >> 4;
		memcpy(e - 3, HexDigits_00_FF + digits * 2, 2);
		value32 &= 0xFu;
	case 1:
		e[-1] = HexDigits_0_F[value32];
		return e;

	case 0:
		s[0] = '0';
		return s + 1;
	}
}

}
