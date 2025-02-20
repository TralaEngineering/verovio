/////////////////////////////////////////////////////////////////////////////
// Name:        supplied.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "supplied.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

static const ClassRegistrar<Supplied> s_factory("supplied", SUPPLIED);

Supplied::Supplied() : EditorialElement(SUPPLIED, "supplied-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Supplied::~Supplied() {}

void Supplied::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
