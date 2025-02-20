/////////////////////////////////////////////////////////////////////////////
// Name:        expand.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expan.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

static const ClassRegistrar<Expan> s_factory("expan", EXPAN);

Expan::Expan() : EditorialElement(EXPAN, "expan-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Expan::~Expan() {}

void Expan::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
