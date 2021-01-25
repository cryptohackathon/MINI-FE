#include "pairings.h"
#include <gmp.h>
const char *Param =
  "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1";
#if PBC_OR_CIFER == 1
void
element_init_G1 (element_t g, pairing_t p)
{
  ECP_BN254_generator (&(g[0].g1));
  mpz_init (g[0].p);
  mpz_init (g[0].z);
  mpz_from_BIG_256_56 (g[0].p, (int64_t *) CURVE_Order_BN254);
  g[0].type = 0;
}

void
element_init_G2 (element_t g, pairing_t p)
{
  ECP2_BN254_generator (&(g[0].g2));
  mpz_init (g[0].p);
  mpz_init (g[0].z);
  mpz_from_BIG_256_56 (g[0].p, (int64_t *) CURVE_Order_BN254);
  g[0].type = 1;
}

void
element_init_GT (element_t g, pairing_t p)
{
  element_init_G1 (g, p);
  element_init_G2 (g, p);
  mpz_init (g[0].z);
  PAIR_BN254_ate (&(g[0].gT), &(g[0].g2), &(g[0].g1));
  PAIR_BN254_fexp (&(g[0].gT));
  mpz_init (g[0].p);
  mpz_from_BIG_256_56 (g[0].p, (int64_t *) CURVE_Order_BN254);
  g[0].type = 2;
}

void
element_init_Zr (element_t g, pairing_t p)
{
  mpz_init (g[0].p);
  mpz_init (g[0].z);
  mpz_from_BIG_256_56 (g[0].p, (int64_t *) CURVE_Order_BN254);
  g[0].type = 4;
}

void
element_random (element_t e)
{
  BIG_256_56 s;
  if (e[0].type == 4)
    {
      cfe_uniform_sample (e[0].z, e[0].p);

    }
  else if (e[0].type == 0)
    {
      mpz_init (e[0].z);
      cfe_uniform_sample (e[0].z, e[0].p);
      BIG_256_56_from_mpz (s, e[0].z);
      ECP_BN254_mul (&(e[0].g1), s);
      mpz_clear (e[0].z);
    }
  else if (e[0].type == 1)
    {
      mpz_init (e[0].z);
      cfe_uniform_sample (e[0].z, e[0].p);
      BIG_256_56_from_mpz (s, e[0].z);
      ECP2_BN254_mul (&(e[0].g2), s);
      mpz_clear (e[0].z);

    }
  else if (e[0].type == 2)
    {
      mpz_init (e[0].z);
      cfe_uniform_sample (e[0].z, e[0].p);
      BIG_256_56_from_mpz (s, e[0].z);
      FP12_BN254_pow (&(e[0].gT), &(e[0].gT), s);
      mpz_clear (e[0].z);

    }

}

void
element_set0 (element_t a)
{
  mpz_set_ui (a[0].z, 0);
// mpz_from_BIG_256_56(a[0].z, (int64_t *) CURVE_Order_BN254);

}

void
element_pow_zn (element_t res, element_t g, element_t y)
{
  BIG_256_56 s;
  ECP_BN254 tmp;
  ECP2_BN254 tmp2;
  mpz_t y2;
  mpz_init (y2);
  if (mpz_cmp_si (y[0].z, 0) == 0)
    mpz_from_BIG_256_56 (y2, (int64_t *) CURVE_Order_BN254);
  else
    mpz_set (y2, y[0].z);
  BIG_256_56_from_mpz (s, y2);
  if (res[0].type == 0)
    {
      ECP_BN254_copy (&tmp, &(g[0].g1));
      ECP_BN254_mul (&tmp, s);
      ECP_BN254_copy (&(res[0]).g1, &tmp);
    }
  else if (res[0].type == 1)
    {
      ECP2_BN254_copy (&tmp2, &(g[0].g2));
      ECP2_BN254_mul (&tmp2, s);
      ECP2_BN254_copy (&(res[0].g2), &tmp2);
    }
  else if (res[0].type == 2)
    {
      FP12_BN254_pow (&(res[0].gT), &(g[0].gT), s);
    }

}

void
element_set1 (element_t e)
{
  element_t g;
  pairing_t p;
//TODO: the following should be an overkill. Use amcl APIs to set the element to zero in a more direct way.
  ECP_BN254 g1;
  ECP2_BN254 g2;
  element_t zero;
  element_init_Zr (zero, p);
  mpz_set_si (zero[0].z, 0);
  if (e[0].type == 0)
    {

      ECP_BN254_generator (&g1);
      ECP_BN254_copy (&(g[0].g1), &g1);
      g[0].type = 0;
      element_pow_zn (e, g, zero);
    }
  else if (e[0].type == 1)
    {
      ECP2_BN254_generator (&g2);
      ECP2_BN254_copy (&(g[0].g2), &g2);
      g[0].type = 1;
      element_pow_zn (e, g, zero);
    }
  else if (e[0].type == 2)
    {
      FP12_BN254_one (&(e[0].gT));
      e[0].type = 2;
    }
  else if (e[0].type == 4)
    {
      mpz_set_ui (e[0].z, 1);
      e[0].type = 4;
    }

}

void
element_set (element_t e, element_t a)
{
  e[0].type = a[0].type;
  mpz_set (e[0].p, a[0].p);
  if (e[0].type == 0)
    ECP_BN254_copy (&(e[0].g1), &(a[0].g1));
  else if (e[0].type == 1)
    ECP2_BN254_copy (&(e[0].g2), &(a[0].g2));
  else if (e[0].type == 2)
    FP12_BN254_copy (&(e[0].gT), &(a[0].gT));
  else if (e[0].type == 4)
    mpz_set (e[0].z, a[0].z);
}

void
element_mul (element_t c, element_t a, element_t b)
{				/* c=ab - we use multiplicative notation for the group */

  ECP_BN254 tmp1;
  ECP2_BN254 tmp2;
  FP12_BN254 tmpT;
  if (c[0].type == 0)
    {
      ECP_BN254_copy (&tmp1, &(a[0].g1));
      ECP_BN254_add (&tmp1, &(b[0].g1));
      ECP_BN254_copy (&(c[0].g1), &tmp1);
    }
  else if (c[0].type == 1)
    {
      ECP2_BN254_copy (&tmp2, &(a[0].g2));
      ECP2_BN254_add (&tmp2, &(b[0].g2));
      ECP2_BN254_copy (&(c[0].g2), &tmp2);
    }
  else if (c[0].type == 2)
    {
      FP12_BN254_copy (&tmpT, &(a[0].gT));
      FP12_BN254_mul (&tmpT, &(b[0].gT));
      FP12_BN254_copy (&(c[0].gT), &tmpT);
    }
  else if (c[0].type == 4)
    {
      mpz_mul (c[0].z, a[0].z, b[0].z);
      mpz_mod (c[0].z, c[0].z, c[0].p);
      if (mpz_cmp_si (c[0].z, 0) == 0)
	mpz_from_BIG_256_56 (c[0].z, (int64_t *) CURVE_Order_BN254);

    }
}

void
element_add (element_t c, element_t a, element_t b)
{				/* c=a+b  */
  if (c[0].type != 4)
    element_mul (c, a, b);
  else
    {
      mpz_add (c[0].z, a[0].z, b[0].z);
      mpz_mod (c[0].z, c[0].z, c[0].p);
      if (mpz_cmp_si (c[0].z, 0) == 0)
	mpz_from_BIG_256_56 (c[0].z, (int64_t *) CURVE_Order_BN254);
    }
}

void
element_sub (element_t c, element_t a, element_t b)
{				/* c=a-b */
  if (c[0].type != 4)
    element_div (c, a, b);
  else
    {
      mpz_sub (c[0].z, a[0].z, b[0].z);
      mpz_mod (c[0].z, c[0].z, c[0].p);
      if (mpz_cmp_si (c[0].z, 0) == 0)
	mpz_from_BIG_256_56 (c[0].z, (int64_t *) CURVE_Order_BN254);
    }
}


void
element_div (element_t c, element_t a, element_t b)
{				/* c=a/b  */
  ECP_BN254 tmp1;
  ECP2_BN254 tmp2;
  FP12_BN254 tmpT, tmpT2;
  if (c[0].type == 0)
    {
      ECP_BN254_copy (&tmp1, &(a[0].g1));
      ECP_BN254_sub (&tmp1, &(b[0].g1));
      ECP_BN254_copy (&(c[0].g1), &tmp1);
    }
  else if (c[0].type == 1)
    {
      ECP2_BN254_copy (&tmp2, &(a[0].g2));
      ECP2_BN254_sub (&tmp2, &(b[0].g2));
      ECP2_BN254_copy (&(c[0].g2), &tmp2);
    }
  else if (c[0].type == 2)
    {
      FP12_BN254_copy (&tmpT, &(a[0].gT));
      FP12_BN254_inv (&tmpT2, &(b[0].gT));
      FP12_BN254_mul (&tmpT, &tmpT2);
      FP12_BN254_copy (&(c[0].gT), &tmpT);
    }

}

int
element_cmp_ui (element_t a, unsigned long int b)
{
  if (mpz_cmp_ui (a[0].z, b) == 0)
    return 1;
  else
    return 0;
}

int
element_cmp_si (element_t a, signed long int b)
{
  if (mpz_cmp_si (a[0].z, b) == 0)
    return 1;
  else
    return 0;
}

int
element_cmp (element_t a, element_t b)
{				/* 0 iff a==b */
  if (a[0].type == 0)
    return !ECP_BN254_equals (&(a[0].g1), &(b[0].g1));
  else if (a[0].type == 1)
    return !ECP2_BN254_equals (&(a[0].g2), &(b[0].g2));
  else if (a[0].type == 2)
    return !FP12_BN254_equals (&(a[0].gT), &(b[0].gT));
  else if (a[0].type == 4)
    {
      if (mpz_cmp (a[0].z, b[0].z) == 0)
	return 0;
      else
	return 1;
    }
  else
    return -1;
}



void
element_set_mpz (element_t a, mpz_t b)
{
  mpz_set (a[0].z, b);
  if (mpz_cmp_si (a[0].z, 0) == 0)
    mpz_from_BIG_256_56 (a[0].z, (int64_t *) CURVE_Order_BN254);
}

void
_element_set_mpz (element_t a, mpz_t b)
{
  mpz_set (a[0].z, b);
}

void
pairing_pp_init (pairing_pp_t pp, element_t x, pairing_t p)
{
  if (x[0].type != 0)
    printf ("error in pairing_pp_init\n");
  pp[0].type = 0;
  mpz_init (pp[0].p);
  mpz_set (pp[0].p, x[0].p);
  ECP_BN254_copy (&(pp[0].g1), &(x[0].g1));
}

void
pairing_pp_clear (pairing_pp_t pp)
{
  return;
}

void
element_pairing (element_t c, element_t a, element_t b)
{				/* c=e(a,b) */
  if (a[0].type == 0 && b[0].type == 1)
    PAIR_BN254_ate (&(c[0].gT), &(b[0].g2), &(a[0].g1));
  else if (a[0].type == 1 && b[0].type == 0)
    PAIR_BN254_ate (&(c[0].gT), &(a[0].g2), &(b[0].g1));
  else
    printf ("error in pairing, check the parameters\n");
  PAIR_BN254_fexp (&(c[0].gT));

}

void
pairing_pp_apply (element_t c, element_t b, pairing_pp_t pp)
{
  element_pairing (c, pp, b);

}

int
element_is1 (element_t a)
{				/* return 1 iff element is unity of the group or integer 1 in case of type 4 */
  if (a[0].type == 2)
    return FP12_BN254_isunity (&(a[0].gT));
  else if (a[0].type == 0)
    return ECP_BN254_isinf (&(a[0].g1));
  else if (a[0].type == 1)
    return ECP2_BN254_isinf (&(a[0].g2));
  else if (a[0].type == 4)
    return !mpz_cmp_ui (a[0].z, 1);
  else
    return -1;
}

/*
void my_OCT_jstring(octet *y,char *s)
{
    int i,j;
    i=y->len;
    j=0;
    y->val=calloc(strlen(s),sizeof(char));
    while (s[j]!=0 && i<y->max)
    {
        y->val[i]=s[j];
        y->len++;
        i++;
        j++;
    }
}
*/

void
hash_G1 (element_t hash, char *str, unsigned int len)
{
  octet tmp_str = { (int) len, 0, str };
  char h[MODBYTES_256_56];
  octet tmp_hash = { 0, sizeof (h), h };
  mhashit (SHA256, -1, &tmp_str, &tmp_hash);
  ECP_BN254_mapit (&(hash[0].g1), &tmp_hash);
}

void
hash_G2 (element_t hash, char *str, unsigned int len)
{
  octet tmp_str = { (int) len, 0, str };
  char h[MODBYTES_256_56];
  octet tmp_hash = { 0, sizeof (h), h };
  mhashit (SHA256, -1, &tmp_str, &tmp_hash);
  ECP2_BN254_mapit (&(hash[0].g2), &tmp_hash);
}

void
hash_Zr (element_t hash, char *str, unsigned int len)
{				/* this function seems not work */
  BIG_256_56 s;
  octet tmp_str = { (int) len, 0, str };
  char h[MODBYTES_256_56 + 1];
  octet tmp_hash = { 0, sizeof (h) - 1, h };
  memset (h, 0, sizeof (h));
  mhashit (SHA256, -1, &tmp_str, &tmp_hash);
  BIG_256_56_fromBytesLen (s, h, sizeof (h) - 1);
  mpz_from_BIG_256_56 (hash[0].z, s);
  mpz_mod (hash[0].z, hash[0].z, hash[0].p);
  //   mpz_import(hash[0].z, sizeof(h)-1, 1, sizeof(h[0]), 0, 0, h);
}


void
element_from_hash (element_t hash, char *str, unsigned int length)
{				/* hash not implemented securely yet  */
  char *str2;
  str2 = calloc (length + 1, sizeof (char));
  memcpy (str2, str, length);
  if (hash[0].type == 4)
    {
      hash_Zr (hash, str2, length);
    }
  else if (hash[0].type == 0)
    hash_G1 (hash, str2, length);
  else if (hash[0].type == 1)
    hash_G2 (hash, str2, length);

  free (str2);
}

int
element_snprintf (char *str, unsigned int length, char *format, element_t g,
		  element_t h, element_t u, element_t v)
{				/* for the moment we support only what needed by the demo to work, that is we ignore format  */
  char *a1, *a2, *a3, *a4;
  char S[2048];
  octet w = { 0, sizeof (S), S };
//  { str[0]='a'; str[1]='\0';   return 4;}
  a1 = calloc (2048, sizeof (char));
  a2 = calloc (2048, sizeof (char));
  a3 = calloc (2048, sizeof (char));
  a4 = calloc (2048, sizeof (char));
  memset (str, 0, length);
  if (g[0].type == 0)
    ECP_BN254_toOctet (&w, &(g[0].g1), 0);
  else if (g[0].type == 2)
    FP12_BN254_toOctet (&w, &(g[0].gT));
  OCT_toStr (&w, a1);
  if (h[0].type == 0)
    ECP_BN254_toOctet (&w, &(h[0].g1), 0);
  else if (h[0].type == 2)
    FP12_BN254_toOctet (&w, &(h[0].gT));
  OCT_toStr (&w, a2);
  if (u[0].type == 0)
    ECP_BN254_toOctet (&w, &(u[0].g1), 0);
  else if (u[0].type == 2)
    FP12_BN254_toOctet (&w, &(u[0].gT));
  OCT_toStr (&w, a3);
  if (v[0].type == 0)
    ECP_BN254_toOctet (&w, &(v[0].g1), 0);
  else if (v[0].type == 2)
    FP12_BN254_toOctet (&w, &(v[0].gT));
  OCT_toStr (&w, a4);
  sprintf (str, "%s%s%s%s", a1, a2, a3, a4);
  free (a1);
  free (a2);
  free (a3);
  free (a4);
  return 4;
}

int
element_snprintf_publickey (char *str, int i, element_t g)
{
  char *a1;
  char S[2048];
  //printf("element_snprintf_publickey %d",g[0].type);
  // { str[0]='a'; str[1]='\0';   return 2;}
  octet w = { 0, sizeof (S), S };
  a1 = calloc (2048, sizeof (char));
  ECP_BN254_toOctet (&w, &(g[0].g1), 0);
  OCT_toStr (&w, a1);
  sprintf (str, "public key of voter %d = %s\n\n", i, a1);
  free (a1);
  return 2;
}

void
pairing_init_set_str (pairing_t p, const char *param)
{
  return;
}


void
element_dlog_brute_force (element_t x, element_t g, element_t h)
{
  pairing_t p;
  element_t i, one;
  element_init_Zr (i, p);
  element_init_Zr (one, p);
  element_t tmp;
  if (g[0].type == 0)
    element_init_G1 (tmp, p);
  else if (g[0].type == 1)
    element_init_G2 (tmp, p);
  else if (g[0].type == 2)
    element_init_GT (tmp, p);
  element_set0 (i);
  element_set1 (one);
  while (1)
    {
      element_pow_zn (tmp, g, i);
      if (element_cmp (tmp, h) == 0)
	{
	  element_set (x, i);

	  return;
	}
      element_add (i, i, one);
    }
  return;
}

void
element_to_mpz (mpz_t z, element_t e)
{
  mpz_set (z, e[0].z);
}

int _write_element_to_file(element_t g,FILE *f_g){
unsigned long int n;
n=sizeof(g[0].g1)+sizeof(g[0].g2)+sizeof(g[0].gT)+sizeof(g[0].x)+sizeof(g[0].type);
if (fwrite((void*)&g[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_alloc);
if (fwrite((void*)&g[0].z[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_size);
if (fwrite((void*)&g[0].z[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].z[0]._mp_alloc*sizeof(mp_limb_t);
if (fwrite((void*)&g[0].z[0]._mp_d[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_alloc);
if (fwrite((void*)&g[0].p[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_size);
if (fwrite((void*)&g[0].p[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].p[0]._mp_alloc*sizeof(mp_limb_t);
if (fwrite((void*)&g[0].p[0]._mp_d[0],1,n,f_g)<=0) return -1;
return 0;
}

int write_element_to_file(element_t g,char *filename){
FILE *f_g;
unsigned long int n;
if ((f_g=fopen(filename,"w+"))==NULL) return -1;
n=sizeof(g[0].g1)+sizeof(g[0].g2)+sizeof(g[0].gT)+sizeof(g[0].x)+sizeof(g[0].type);
if (fwrite((void*)&g[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_alloc);
if (fwrite((void*)&g[0].z[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_size);
if (fwrite((void*)&g[0].z[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].z[0]._mp_alloc*sizeof(mp_limb_t);
if (fwrite((void*)&g[0].z[0]._mp_d[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_alloc);
if (fwrite((void*)&g[0].p[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_size);
if (fwrite((void*)&g[0].p[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].p[0]._mp_alloc*sizeof(mp_limb_t);
if (fwrite((void*)&g[0].p[0]._mp_d[0],1,n,f_g)<=0) return -1;
if (fclose(f_g)!=0) return -1;
return 0;
}

int _read_element_from_file(element_t g,FILE*f_g){
unsigned long int n;
n=sizeof(g[0].g1)+sizeof(g[0].g2)+sizeof(g[0].gT)+sizeof(g[0].x)+sizeof(g[0].type);
if (fread((void*)&g[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_alloc);
if (fread((void*)&g[0].z[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_size);
if (fread((void*)&g[0].z[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].z[0]._mp_alloc*sizeof(mp_limb_t);
g[0].z[0]._mp_d=malloc(n);
if (fread((void*)&g[0].z[0]._mp_d[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_alloc);
if (fread((void*)&g[0].p[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_size);
if (fread((void*)&g[0].p[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].p[0]._mp_alloc*sizeof(mp_limb_t);
g[0].p[0]._mp_d=malloc(n);
if (fread((void*)&g[0].p[0]._mp_d[0],1,n,f_g)<=0) return -1;
return 0;
}
int read_element_from_file(element_t g,char *filename){
FILE *f_g;
unsigned long int n;
if ((f_g=fopen(filename,"r"))==NULL) return -1;
//_read_element_from_file(g,f_g);

n=sizeof(g[0].g1)+sizeof(g[0].g2)+sizeof(g[0].gT)+sizeof(g[0].x)+sizeof(g[0].type);
if (fread((void*)&g[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_alloc);
if (fread((void*)&g[0].z[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].z[0]._mp_size);
if (fread((void*)&g[0].z[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].z[0]._mp_alloc*sizeof(mp_limb_t);
g[0].z[0]._mp_d=malloc(n);
if (fread((void*)&g[0].z[0]._mp_d[0],1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_alloc);
if (fread((void*)&g[0].p[0]._mp_alloc,1,n,f_g)<=0) return -1;
n=sizeof(g[0].p[0]._mp_size);
if (fread((void*)&g[0].p[0]._mp_size,1,n,f_g)<=0) return -1;
n=g[0].p[0]._mp_alloc*sizeof(mp_limb_t);
g[0].p[0]._mp_d=malloc(n);
if (fread((void*)&g[0].p[0]._mp_d[0],1,n,f_g)<=0) return -1;
if (fclose(f_g)!=0) return -1;
return 0;
}

#endif
