/* Copyright 2023 Cipulot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

#define ___ KC_NO

// clang-format off
#define LAYOUT( \
                     k00, k01, k02, k03, k04, k05, k25, k24, k23, k22, k21, k20,                            \
                     k10, k11, k12, k13, k14, k15, k35, k34, k33, k32, k31, k30,                            \
    k40, k41,   k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G,   k4H, k4I, k4J, \
    k50, k51,   k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F,        k5H, k5I, k5J, \
    k60, k61,   k62, k63, k64, k65, k66, k67, k68, k69, k6A, k6B, k6C, k6D, k6E, k6F, k6G,   k6H, k6I, k6J, \
    k70, k71,   k72, k73, k74, k75, k76, k77, k78, k79, k7A, k7B, k7C, k7D, k7E, k7F,        k7H, k7I, k7J, \
    k80, k81,   k82, k83, k84,                     k89,                     k8E, k8F, k8G,   k8H, k8I, k8J  \
) { \
	{k00, k01, k02, k03, k04, k05, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___}, \
	{k10, k11, k12, k13, k14, k15, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___}, \
	{k20, k21, k22, k23, k24, k25, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___}, \
	{k30, k31, k32, k33, k34, k35, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___, ___}, \
	{k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H, k4I, k4J}, \
	{k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k5A, k5B, k5C, k5D, k5E, k5F, ___, k5H, k5I, k5J}, \
	{k60, k61, k62, k63, k64, k65, k66, k67, k68, k69, k6A, k6B, k6C, k6D, k6E, k6F, k6G, k6H, k6I, k6J}, \
	{k70, k71, k72, k73, k74, k75, k76, k77, k78, k79, k7A, k7B, k7C, k7D, k7E, k7F, ___, k7H, k7I, k7J}, \
	{k80, k81, k82, k83, k84, ___, ___, ___, ___, k89, ___, ___, ___, ___, k8E, k8F, k8G, k8H, k8I, k8J} \
}
// clang-format on
