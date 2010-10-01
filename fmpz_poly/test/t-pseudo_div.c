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

    Copyright (C) 2009 William Hart

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    fmpz_randstate_t state;

    printf("pseudo_div....");
    fflush(stdout);

    fmpz_poly_randinit(state);

    /* Check r = a - q * b has small degree, no aliasing */
    for (i = 0; i < 2000; i++)
    {
        fmpz_poly_t a, b, q, r, prod;
        fmpz_t p;
        ulong d;

        fmpz_init(p);
        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(q);
        fmpz_poly_init(r);
        fmpz_poly_init(prod);
        fmpz_poly_randtest(a, state, n_randint(100), 50);
        fmpz_poly_randtest_not_zero(b, state, n_randint(100) + 1, 50);

        fmpz_poly_pseudo_div(q, &d, a, b);
        fmpz_poly_mul(prod, q, b);
        fmpz_pow_ui(p, b->coeffs + b->length - 1, d);
        fmpz_poly_scalar_mul_fmpz(a, a, p);
        fmpz_poly_sub(r, a, prod);
        
        result = (fmpz_poly_length(r) < fmpz_poly_length(b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_print(a), printf("\n\n");
            fmpz_poly_print(prod), printf("\n\n");
            fmpz_poly_print(q), printf("\n\n");
            fmpz_poly_print(r), printf("\n\n");
            abort();
        }

        fmpz_clear(p);
        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(q);
        fmpz_poly_clear(r);
        fmpz_poly_clear(prod);
    }

    /* Check q and a alias */
    for (i = 0; i < 500; i++)
    {
        fmpz_poly_t a, b, q;
        ulong d;
        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(q);
        fmpz_poly_randtest(a, state, n_randint(100), 50);
        fmpz_poly_randtest_not_zero(b, state, n_randint(100) + 1, 50);

        fmpz_poly_pseudo_div(q, &d, a, b);
        fmpz_poly_pseudo_div(a, &d, a, b);

        result = (fmpz_poly_equal(a, q));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_print(a), printf("\n\n");
            fmpz_poly_print(q), printf("\n\n");
            abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(q);
    }

    /* Check q and b alias */
    for (i = 0; i < 500; i++)
    {
        fmpz_poly_t a, b, q;
        ulong d;

        fmpz_poly_init(a);
        fmpz_poly_init(b);
        fmpz_poly_init(q);
        fmpz_poly_randtest(a, state, n_randint(100), 50);
        fmpz_poly_randtest_not_zero(b, state, n_randint(100) + 1, 50);

        fmpz_poly_pseudo_div(q, &d, a, b);
        fmpz_poly_pseudo_div(b, &d, a, b);

        result = (fmpz_poly_equal(b, q));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_print(a), printf("\n\n");
            fmpz_poly_print(q), printf("\n\n");
            abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(b);
        fmpz_poly_clear(q);
    }

    fmpz_poly_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
