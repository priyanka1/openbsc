/* (C) 2008 by Jan Luebbe <jluebbe@debian.org>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <openbsc/db.h>

#include <stdio.h>

int main() {

    if (db_init()) {
        printf("DB: Failed to init database. Please check the option settings.\n");
        return 1;
    }    
    printf("DB: Database initialized.\n");

    if (db_prepare()) {
        printf("DB: Failed to prepare database.\n");
        return 1;
    }
    printf("DB: Database prepared.\n");

    db_insert_imsi(3243245432351LLU);
    db_insert_imsi(3243245432352LLU);
    db_imsi_set_tmsi(3243245432345LLU, 99999999LLU);
    db_imsi_set_lac(3243245432345LLU, 42);

    db_subscriber alice;
    db_imsi_get_subscriber(3243245432345LLU, &alice);
    db_tmsi_get_subscriber(99999999LLU, &alice);

    db_fini();

    return 0;
}
