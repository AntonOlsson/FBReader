/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __TEXTELEMENT_H__
#define __TEXTELEMENT_H__

#include "../model/TextKind.h"
#include "../model/Paragraph.h"

class TextElement {

protected:
	TextElement() VIEW_SECTION;

public:
	virtual ~TextElement() VIEW_SECTION;

	enum Kind {
		WORD_ELEMENT = 0x01,
		IMAGE_ELEMENT = 0x02,
		CONTROL_ELEMENT = 0x03,
		TREE_ELEMENT = 0x04,

		INDENT_ELEMENT = 0x11,
		HSPACE_ELEMENT = 0x12,
		BEFORE_PARAGRAPH_ELEMENT = 0x13,
		AFTER_PARAGRAPH_ELEMENT = 0x14,
		EMPTY_LINE_ELEMENT = 0x15,
	};

	virtual Kind kind() const VIEW_SECTION = 0;

private:
	// assignment and copy constructor are disabled
	TextElement(const TextElement&) VIEW_SECTION;
	TextElement &operator = (const TextElement&) VIEW_SECTION;
};

class Image;

class ImageElement : public TextElement {

public:
	ImageElement(const Image &image) VIEW_SECTION;
	~ImageElement() VIEW_SECTION;
	Kind kind() const VIEW_SECTION;
	const Image &image() const VIEW_SECTION;

private:
	const Image &myImage;
};

class SpecialTextElement : public TextElement {

public:
	SpecialTextElement(Kind kind) VIEW_SECTION;
	~SpecialTextElement() VIEW_SECTION;
	Kind kind() const VIEW_SECTION;

private:
	Kind myKind;
};

class ControlElement : public TextElement {

public:
	ControlElement(const ControlEntry &entry) VIEW_SECTION;
	~ControlElement() VIEW_SECTION;
	Kind kind() const VIEW_SECTION;
	const ControlEntry &entry() const VIEW_SECTION;
	TextKind textKind() const VIEW_SECTION;
	bool isStart() const VIEW_SECTION;

private:
	const ControlEntry &myEntry;
};

class TreeElement : public TextElement {

public:
	enum TreeElementKind {
		TREE_ELEMENT_OPEN_NODE,
		TREE_ELEMENT_CLOSED_NODE,
		TREE_ELEMENT_LEAF,
		TREE_ELEMENT_TOP_RIGHT_LINE,
		TREE_ELEMENT_TOP_BOTTOM_RIGHT_LINE,
		TREE_ELEMENT_SKIP,
	};

	TreeElement(TreeElementKind treeElementKind) VIEW_SECTION;
	~TreeElement() VIEW_SECTION;
	Kind kind() const VIEW_SECTION;
	TreeElementKind treeElementKind() const VIEW_SECTION;

private:
	TreeElementKind myTreeElementKind;
};

inline TextElement::TextElement() {}
inline TextElement::~TextElement() {}

inline ImageElement::ImageElement(const Image &image) : myImage(image) {}
inline ImageElement::~ImageElement() {}
inline TextElement::Kind ImageElement::kind() const { return IMAGE_ELEMENT; };
inline const Image &ImageElement::image() const { return myImage; }

inline SpecialTextElement::SpecialTextElement(Kind kind) : myKind(kind) {}
inline SpecialTextElement::~SpecialTextElement() {}
inline TextElement::Kind SpecialTextElement::kind() const { return myKind; };

inline ControlElement::ControlElement(const ControlEntry &entry) : myEntry(entry) {}
inline ControlElement::~ControlElement() {}
inline TextElement::Kind ControlElement::kind() const { return CONTROL_ELEMENT; };
inline const ControlEntry &ControlElement::entry() const { return myEntry; }
inline TextKind ControlElement::textKind() const { return myEntry.kind(); }
inline bool ControlElement::isStart() const { return myEntry.isStart(); }

inline TreeElement::TreeElement(TreeElementKind treeElementKind) : myTreeElementKind(treeElementKind) {}
inline TreeElement::~TreeElement() {}
inline TextElement::Kind TreeElement::kind() const { return TREE_ELEMENT; };
inline TreeElement::TreeElementKind TreeElement::treeElementKind() const { return myTreeElementKind; }

#endif /* __TEXTELEMENT_H__ */
