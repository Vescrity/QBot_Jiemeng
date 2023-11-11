#ifndef __MD5H__
#define __MD5H__
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct
{
	UINT4 i[2];								/* number of _bits_ handled mod 2^64 */
	UINT4 buf[4];							/* scratch buffer */
	unsigned char in[64];			/* input buffer */
	unsigned char digest[16]; /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init(MD5_CTX *mdContext);
void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *mdContext);

static void Transform();
static void TransformMD5(UINT4 *buf, UINT4 *in);

static unsigned char PADDING[64] = {
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define _F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define _G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define _H(x, y, z) ((x) ^ (y) ^ (z))
#define _I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define _FF(a, b, c, d, x, s, ac)                 \
	{                                               \
		(a) += _F((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT((a), (s));                  \
		(a) += (b);                                   \
	}
#define _GG(a, b, c, d, x, s, ac)                 \
	{                                               \
		(a) += _G((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT((a), (s));                  \
		(a) += (b);                                   \
	}
#define _HH(a, b, c, d, x, s, ac)                 \
	{                                               \
		(a) += _H((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT((a), (s));                  \
		(a) += (b);                                   \
	}
#define _II(a, b, c, d, x, s, ac)                 \
	{                                               \
		(a) += _I((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT((a), (s));                  \
		(a) += (b);                                   \
	}

void MD5Init(MD5_CTX *mdContext)
{
	mdContext->i[0] = mdContext->i[1] = (UINT4)0;

	/* Load magic initialization constants.
	 */
	mdContext->buf[0] = (UINT4)0x67452301;
	mdContext->buf[1] = (UINT4)0xefcdab89;
	mdContext->buf[2] = (UINT4)0x98badcfe;
	mdContext->buf[3] = (UINT4)0x10325476;
}

void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* update number of bits */
	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
		mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);

	while (inLen--)
	{
		/* add new character to buffer,increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* transform if necessary */
		if (mdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
								(((UINT4)mdContext->in[ii + 2]) << 16) |
								(((UINT4)mdContext->in[ii + 1]) << 8) |
								((UINT4)mdContext->in[ii]);
			TransformMD5(mdContext->buf, in);
			mdi = 0;
		}
	}
}

void MD5Final(MD5_CTX *mdContext)

{
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;
	unsigned int padLen;

	/* save number of bits */
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* pad out to 56 mod 64 */
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	MD5Update(mdContext, PADDING, padLen);

	/* append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
		in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
						(((UINT4)mdContext->in[ii + 2]) << 16) |
						(((UINT4)mdContext->in[ii + 1]) << 8) |
						((UINT4)mdContext->in[ii]);
	TransformMD5(mdContext->buf, in);

	/* store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4)
	{
		mdContext->digest[ii] = (unsigned char)(mdContext->buf[i] & 0xFF);
		mdContext->digest[ii + 1] =
				(unsigned char)((mdContext->buf[i] >> 8) & 0xFF);
		mdContext->digest[ii + 2] =
				(unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii + 3] =
				(unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}

/* Basic MD5 step. Transform buf based on in.
 */
static void TransformMD5(UINT4 *buf, UINT4 *in)
{
	UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
	_FF(a, b, c, d, in[0], S11, 3614090360);	/* 1 */
	_FF(d, a, b, c, in[1], S12, 3905402710);	/* 2 */
	_FF(c, d, a, b, in[2], S13, 606105819);		/* 3 */
	_FF(b, c, d, a, in[3], S14, 3250441966);	/* 4 */
	_FF(a, b, c, d, in[4], S11, 4118548399);	/* 5 */
	_FF(d, a, b, c, in[5], S12, 1200080426);	/* 6 */
	_FF(c, d, a, b, in[6], S13, 2821735955);	/* 7 */
	_FF(b, c, d, a, in[7], S14, 4249261313);	/* 8 */
	_FF(a, b, c, d, in[8], S11, 1770035416);	/* 9 */
	_FF(d, a, b, c, in[9], S12, 2336552879);	/* 10 */
	_FF(c, d, a, b, in[10], S13, 4294925233); /* 11 */
	_FF(b, c, d, a, in[11], S14, 2304563134); /* 12 */
	_FF(a, b, c, d, in[12], S11, 1804603682); /* 13 */
	_FF(d, a, b, c, in[13], S12, 4254626195); /* 14 */
	_FF(c, d, a, b, in[14], S13, 2792965006); /* 15 */
	_FF(b, c, d, a, in[15], S14, 1236535329); /* 16 */

	/* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
	_GG(a, b, c, d, in[1], S21, 4129170786);	/* 17 */
	_GG(d, a, b, c, in[6], S22, 3225465664);	/* 18 */
	_GG(c, d, a, b, in[11], S23, 643717713);	/* 19 */
	_GG(b, c, d, a, in[0], S24, 3921069994);	/* 20 */
	_GG(a, b, c, d, in[5], S21, 3593408605);	/* 21 */
	_GG(d, a, b, c, in[10], S22, 38016083);		/* 22 */
	_GG(c, d, a, b, in[15], S23, 3634488961); /* 23 */
	_GG(b, c, d, a, in[4], S24, 3889429448);	/* 24 */
	_GG(a, b, c, d, in[9], S21, 568446438);		/* 25 */
	_GG(d, a, b, c, in[14], S22, 3275163606); /* 26 */
	_GG(c, d, a, b, in[3], S23, 4107603335);	/* 27 */
	_GG(b, c, d, a, in[8], S24, 1163531501);	/* 28 */
	_GG(a, b, c, d, in[13], S21, 2850285829); /* 29 */
	_GG(d, a, b, c, in[2], S22, 4243563512);	/* 30 */
	_GG(c, d, a, b, in[7], S23, 1735328473);	/* 31 */
	_GG(b, c, d, a, in[12], S24, 2368359562); /* 32 */

	/* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
	_HH(a, b, c, d, in[5], S31, 4294588738);	/* 33 */
	_HH(d, a, b, c, in[8], S32, 2272392833);	/* 34 */
	_HH(c, d, a, b, in[11], S33, 1839030562); /* 35 */
	_HH(b, c, d, a, in[14], S34, 4259657740); /* 36 */
	_HH(a, b, c, d, in[1], S31, 2763975236);	/* 37 */
	_HH(d, a, b, c, in[4], S32, 1272893353);	/* 38 */
	_HH(c, d, a, b, in[7], S33, 4139469664);	/* 39 */
	_HH(b, c, d, a, in[10], S34, 3200236656); /* 40 */
	_HH(a, b, c, d, in[13], S31, 681279174);	/* 41 */
	_HH(d, a, b, c, in[0], S32, 3936430074);	/* 42 */
	_HH(c, d, a, b, in[3], S33, 3572445317);	/* 43 */
	_HH(b, c, d, a, in[6], S34, 76029189);		/* 44 */
	_HH(a, b, c, d, in[9], S31, 3654602809);	/* 45 */
	_HH(d, a, b, c, in[12], S32, 3873151461); /* 46 */
	_HH(c, d, a, b, in[15], S33, 530742520);	/* 47 */
	_HH(b, c, d, a, in[2], S34, 3299628645);	/* 48 */

	/* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
	_II(a, b, c, d, in[0], S41, 4096336452);	/* 49 */
	_II(d, a, b, c, in[7], S42, 1126891415);	/* 50 */
	_II(c, d, a, b, in[14], S43, 2878612391); /* 51 */
	_II(b, c, d, a, in[5], S44, 4237533241);	/* 52 */
	_II(a, b, c, d, in[12], S41, 1700485571); /* 53 */
	_II(d, a, b, c, in[3], S42, 2399980690);	/* 54 */
	_II(c, d, a, b, in[10], S43, 4293915773); /* 55 */
	_II(b, c, d, a, in[1], S44, 2240044497);	/* 56 */
	_II(a, b, c, d, in[8], S41, 1873313359);	/* 57 */
	_II(d, a, b, c, in[15], S42, 4264355552); /* 58 */
	_II(c, d, a, b, in[6], S43, 2734768916);	/* 59 */
	_II(b, c, d, a, in[13], S44, 1309151649); /* 60 */
	_II(a, b, c, d, in[4], S41, 4149444226);	/* 61 */
	_II(d, a, b, c, in[11], S42, 3174756917); /* 62 */
	_II(c, d, a, b, in[2], S43, 718787259);		/* 63 */
	_II(b, c, d, a, in[9], S44, 3951481745);	/* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}
#include <string>
using namespace std;
string md5(const string &strPlain)
{
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	MD5Init(&mdContext);
	MD5Update(&mdContext, (unsigned char *)const_cast<char *>(strPlain.c_str()), strPlain.size());
	MD5Final(&mdContext);
	string md5;
	char buf[3];
	for (int i = 0; i < 16; i++)
	{
		sprintf(buf, "%02x", mdContext.digest[i]);
		md5.append(buf);
	}
	return md5;
}
#endif
