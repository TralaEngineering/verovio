/////////////////////////////////////////////////////////////////////////////
// Name:        accid.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "accid.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functorparams.h"
#include "note.h"
#include "smufl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

static const ClassRegistrar<Accid> s_factory("accid", ACCID);

Accid::Accid()
    : LayerElement(ACCID, "accid-")
    , PositionInterface()
    , AttAccidental()
    , AttAccidentalGestural()
    , AttAccidLog()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSym()
{

    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_ACCIDENTALGESTURAL);
    RegisterAttClass(ATT_ACCIDLOG);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ENCLOSINGCHARS);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Accid::~Accid() {}

void Accid::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetAccidental();
    ResetAccidentalGestural();
    ResetAccidLog();
    ResetColor();
    ResetEnclosingChars();
    ResetExtSym();

    m_drawingUnison = NULL;
}

std::wstring Accid::GetSymbolStr() const
{
    if (!this->HasAccid()) return L"";

    wchar_t code = 0;

    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        code = GetGlyphNum();
        if (NULL == Resources::GetGlyph(code)) code = 0;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL == Resources::GetGlyph(code)) code = 0;
    }

    if (code == 0) code = GetAccidGlyph(this->GetAccid());
    std::wstring symbolStr;

    if (this->HasEnclose()) {
        if (this->GetEnclose() == ENCLOSURE_brack) {
            symbolStr.push_back(SMUFL_E26C_accidentalBracketLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26D_accidentalBracketRight);
        }
        else {
            symbolStr.push_back(SMUFL_E26A_accidentalParensLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26B_accidentalParensRight);
        }
    }
    else {
        symbolStr.push_back(code);
    }
    return symbolStr;
}

bool Accid::AdjustX(LayerElement *element, Doc *doc, int staffSize, std::vector<Accid *> &leftAccids)
{
    assert(element);
    assert(doc);

    if (this == element) return false;

    int verticalMargin = 1 * doc->GetDrawingStemWidth(staffSize);
    int horizontalMargin = 2 * doc->GetDrawingStemWidth(staffSize);

    if (element->Is(NOTE)) horizontalMargin = 3 * doc->GetDrawingStemWidth(staffSize);

    if (!this->VerticalSelfOverlap(element, verticalMargin)) return false;

    // Look for identical accidentals that needs to remain superimposed
    if (element->Is(ACCID) && (this->GetDrawingY() == element->GetDrawingY())) {
        Accid *accid = vrv_cast<Accid *>(element);
        assert(accid);
        if (this->GetSymbolStr() == accid->GetSymbolStr()) {
            // There is the same accidental, so we leave it a the same place
            // This should also work for the chords on multiple layers by setting unison accidental
            accid->SetDrawingUnisonAccid(this);
            return false;
        }
    }

    if (element->Is(ACCID)) {
        if (!this->HorizontalLeftOverlap(element, doc, horizontalMargin, verticalMargin)) {
            // There is enough space on the right of the accidental, but maybe we will need to
            // adjust it again (see recursive call below), so keep the accidental that is on the left
            leftAccids.push_back(dynamic_cast<Accid *>(element));
            return false;
        }
    }

    int xRelShift = 0;
    if (element->Is(STEM))
        xRelShift = this->GetSelfRight() - element->GetSelfLeft() + horizontalMargin;
    else
        xRelShift = this->HorizontalRightOverlap(element, doc, horizontalMargin, verticalMargin);

    // Move only to the left
    if (xRelShift > 0) {
        this->SetDrawingXRel(this->GetDrawingXRel() - xRelShift);
        // We have some accidentals on the left, check again with all of these
        if (!leftAccids.empty()) {
            std::vector<Accid *> leftAccidsSubset;
            std::vector<Accid *>::iterator iter;
            // Recursively adjust all accidental that are on the left because enough space was previously available
            for (iter = leftAccids.begin(); iter != leftAccids.end(); ++iter) {
                this->AdjustX(dynamic_cast<LayerElement *>(*iter), doc, staffSize, leftAccidsSubset);
            }
        }
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
// Static methods for Accid
//----------------------------------------------------------------------------

wchar_t Accid::GetAccidGlyph(data_ACCIDENTAL_WRITTEN accid)
{
    int symc = SMUFL_E261_accidentalNatural;
    switch (accid) {
        case ACCIDENTAL_WRITTEN_s: symc = SMUFL_E262_accidentalSharp; break;
        case ACCIDENTAL_WRITTEN_f: symc = SMUFL_E260_accidentalFlat; break;
        case ACCIDENTAL_WRITTEN_ss: symc = SMUFL_E269_accidentalSharpSharp; break;
        case ACCIDENTAL_WRITTEN_x: symc = SMUFL_E263_accidentalDoubleSharp; break;
        case ACCIDENTAL_WRITTEN_ff: symc = SMUFL_E264_accidentalDoubleFlat; break;
        case ACCIDENTAL_WRITTEN_sx: symc = SMUFL_E265_accidentalTripleSharp; break; // Missing in SMuFL
        case ACCIDENTAL_WRITTEN_xs: symc = SMUFL_E265_accidentalTripleSharp; break;
        case ACCIDENTAL_WRITTEN_ts: symc = SMUFL_E265_accidentalTripleSharp; break; // Missing in SMuFL
        case ACCIDENTAL_WRITTEN_tf: symc = SMUFL_E266_accidentalTripleFlat; break;
        case ACCIDENTAL_WRITTEN_n: symc = SMUFL_E261_accidentalNatural; break;
        case ACCIDENTAL_WRITTEN_nf: symc = SMUFL_E267_accidentalNaturalFlat; break;
        case ACCIDENTAL_WRITTEN_ns: symc = SMUFL_E268_accidentalNaturalSharp; break;
        case ACCIDENTAL_WRITTEN_su: symc = SMUFL_E274_accidentalThreeQuarterTonesSharpArrowUp; break;
        case ACCIDENTAL_WRITTEN_sd: symc = SMUFL_E275_accidentalQuarterToneSharpArrowDown; break;
        case ACCIDENTAL_WRITTEN_fu: symc = SMUFL_E270_accidentalQuarterToneFlatArrowUp; break;
        case ACCIDENTAL_WRITTEN_fd: symc = SMUFL_E271_accidentalThreeQuarterTonesFlatArrowDown; break;
        case ACCIDENTAL_WRITTEN_nu: symc = SMUFL_E272_accidentalQuarterToneSharpNaturalArrowUp; break;
        case ACCIDENTAL_WRITTEN_nd: symc = SMUFL_E273_accidentalQuarterToneFlatNaturalArrowDown; break;
        case ACCIDENTAL_WRITTEN_1qf: symc = SMUFL_E280_accidentalQuarterToneFlatStein; break;
        case ACCIDENTAL_WRITTEN_3qf: symc = SMUFL_E281_accidentalThreeQuarterTonesFlatZimmermann; break;
        case ACCIDENTAL_WRITTEN_1qs: symc = SMUFL_E282_accidentalQuarterToneSharpStein; break;
        case ACCIDENTAL_WRITTEN_3qs: symc = SMUFL_E283_accidentalThreeQuarterTonesSharpStein; break;
        case ACCIDENTAL_WRITTEN_bms: symc = SMUFL_E447_accidentalBuyukMucennebSharp; break;
        case ACCIDENTAL_WRITTEN_kms: symc = SMUFL_E446_accidentalKucukMucennebSharp; break;
        case ACCIDENTAL_WRITTEN_bs: symc = SMUFL_E445_accidentalBakiyeSharp; break;
        case ACCIDENTAL_WRITTEN_ks: symc = SMUFL_E444_accidentalKomaSharp; break;
        case ACCIDENTAL_WRITTEN_kf: symc = SMUFL_E443_accidentalKomaFlat; break;
        case ACCIDENTAL_WRITTEN_bf: symc = SMUFL_E442_accidentalBakiyeFlat; break;
        case ACCIDENTAL_WRITTEN_kmf: symc = SMUFL_E441_accidentalKucukMucennebFlat; break;
        case ACCIDENTAL_WRITTEN_bmf: symc = SMUFL_E440_accidentalBuyukMucennebFlat; break;
        default: break;
    }
    return symc;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Accid::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Accid::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    m_isDrawingOctave = false;
    m_drawingOctave = NULL;
    m_drawingUnison = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
