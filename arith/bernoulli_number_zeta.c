/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <mpir.h>
#include <mpfr.h>
#include "flint.h"
#include "arith.h"
#include "ulong_extras.h"

void bernoulli_number_zeta(fmpz_t num, fmpz_t den, ulong n)
{
    mpz_t r;
    mpfr_t t, u, z, pi;
    long prec, pi_prec;

    if (n % 2)
    {
        fmpz_set_si(num, -(n == 1));
        fmpz_set_ui(den, 1 + (n == 1));
        return;
    }

    if (n < SMALL_BERNOULLI_LIMIT)
    {
        fmpz_set_si(num, bernoulli_numer_small[n / 2]);
        fmpz_set_ui(den, bernoulli_denom_small[n / 2]);
        return;
    }

    fmpz_bernoulli_denom(den, n);

    prec = bernoulli_number_size(n) + fmpz_bits(den) + 10;
    pi_prec = prec + FLINT_BIT_COUNT(n);

    mpz_init(r);
    mpfr_init2(t, prec);
    mpfr_init2(u, prec);
    mpfr_init2(z, prec);
    mpfr_init2(pi, pi_prec);

    /* t = 2 * n! / (2*pi)^n */
    mpz_fac_ui(r, n);
    mpfr_set_z(t, r, GMP_RNDN);
    mpfr_mul_2exp(t, t, 1, GMP_RNDN);
    mpfr_const_pi(pi, GMP_RNDN);
    mpfr_mul_2exp(pi, pi, 1, GMP_RNDN);
    mpfr_pow_ui(pi, pi, n, GMP_RNDN);
    mpfr_div(t, t, pi, GMP_RNDN);

    /* t = t / zeta(n) */
    _zeta_inv_euler_product(z, n, 0);
    mpfr_div(t, t, z, GMP_RNDN);

    /* round numerator */
    fmpz_get_mpz(r, den);
    mpfr_mul_z(t, t, r, GMP_RNDN);
    mpfr_round(t, t);
    mpfr_get_z(r, t, GMP_RNDN);
    fmpz_set_mpz(num, r);

    if (n % 4 == 0)
        fmpz_neg(num, num);

    mpz_clear(r);
    mpfr_clear(t);
    mpfr_clear(u);
    mpfr_clear(z);
    mpfr_clear(pi);
}
