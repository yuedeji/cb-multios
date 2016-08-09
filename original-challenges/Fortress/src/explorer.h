/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#pragma once

#include "skill.h"

class cgc_CExplorer
{
    public:
        cgc_CExplorer(const char *name);
        ~cgc_CExplorer() {};

        const char* cgc_GetName() { return m_name; }
        int cgc_GetLevel() { return m_level; }
        int cgc_GetExp() { return m_exp; }
        int cgc_GetNextExp();
        bool cgc_GainExp(int exp);
        void cgc_ReplaceSkill(cgc_CSkill* skill, int idx);
        cgc_CRequirement::Type cgc_GetCounters();
        bool cgc_IsAvail() { return m_avail; }
        bool cgc_IsHired() { return m_hired; }
        void cgc_SetHired(bool flag) { m_hired = flag; }
        void cgc_SetAvail(bool flag) { m_avail = flag; }
        cgc_CSkill** cgc_GetSkills() { return m_skills; }
        virtual void cgc_ChangeName(const char *name);
        static const int k_maxNameLength = 16;

    private:
        static const int k_maxLevel = 100;
        static const int k_baseExp = 1000;
        constexpr static const double k_nextExpMultiplier = 1.15;

        int m_exp;
        int m_level;
        cgc_CSkill* m_skills[2];
        char m_name[k_maxNameLength + 1];
        bool m_avail;
        bool m_hired;
};
