/*
 * Copyright (C) 2007 Nikolay Pultsin <geometer@mawhrin.net>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <iostream>

#include "W32Container.h"

W32Table::W32Table() : myVerticalSpacing(0), myHorizontalSpacing(0) {
}

void W32Table::setSpacings(int vertical, int horizontal) {
	myVerticalSpacing = vertical;
	myHorizontalSpacing = horizontal;
}

void W32Table::setElement(W32WidgetPtr element, int row, int fromColumn, int toColumn) {
	if (!element.isNull()) {
		if (row >= (int)myRows.size()) {
			myRows.insert(myRows.end(), row - myRows.size() + 1, RowList());
		}
		RowList &rowList = myRows[row];

		RowList::iterator it = rowList.begin();
		bool canInsertAfter = true;
		for (; it != rowList.end(); ++it) {
			if (it->XFrom > toColumn) {
				break;
			}
			canInsertAfter = it->XTo < fromColumn;
		}
		if (canInsertAfter) {
			rowList.insert(it, CellInfo(fromColumn, toColumn, element));
		}
	}
}

void W32Table::allocate(WORD *&p, short &id) const {
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
			jt->Widget->allocate(p, id);
		}
	}
}

int W32Table::allocationSize() const {
	int size = 0;
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
			size += jt->Widget->allocationSize();
		}
	}
	return size;
}

void W32Table::setVisible(bool visible) {
	// TODO: implement
}

bool W32Table::isVisible() const {
	// TODO: implement
	return true;
}

int W32Table::controlNumber() const {
	int counter = 0;
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
			counter += jt->Widget->controlNumber();
		}
	}
	return counter;
}

void W32Table::calculateSizes(std::vector<short> &widths, std::vector<short> &heights) const {
	//std::cerr << "calculateSizes...";
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		short currentHeight = 0;
		if (!it->empty()) {
			for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
				if (jt->Widget->isVisible()) {
					Size elementSize = jt->Widget->minimumSize();
					currentHeight = std::max(currentHeight, elementSize.Height);
					size_t column = jt->XFrom;
					if (column >= widths.size()) {
						widths.insert(widths.end(), column - widths.size() + 1, 0);
					}
					widths[column] = std::max(widths[column], elementSize.Width); 
				}
			}
		}
		heights.push_back(currentHeight);
	}
	//std::cerr << "OK\n";
	/*
	std::cerr << "widths: ";
	for (size_t i = 0; i < widths.size(); ++i) {
		std::cerr << widths[i] << ' ';
	}
	std::cerr << "\n";
	std::cerr << "heights: ";
	for (size_t i = 0; i < heights.size(); ++i) {
		std::cerr << heights[i] << ' ';
	}
	std::cerr << "\n";
	*/
}

W32Widget::Size W32Table::minimumSize() const {
	std::vector<short> widths, heights;
	calculateSizes(widths, heights);

	Size size(leftMargin() + rightMargin(), topMargin() + bottomMargin());

	int wCount = 0;
	for (std::vector<short>::const_iterator it = widths.begin(); it != widths.end(); ++it) {
		size.Width += *it;
		if (*it != 0) {
			++wCount;
		}
	}
	if (wCount > 1) {
		size.Width += myHorizontalSpacing * (wCount - 1);
	}

	int hCount = 0;
	for (std::vector<short>::const_iterator it = heights.begin(); it != heights.end(); ++it) {
		size.Height += *it;
		if (*it != 0) {
			++hCount;
		}
	}
	if (hCount > 1) {
		size.Height += myVerticalSpacing * (hCount - 1);
	}

	return size;
}

void W32Table::setPosition(int x, int y, Size size) {
	std::vector<short> widths, heights, lefts;
	calculateSizes(widths, heights);

	Size minSize(leftMargin() + rightMargin(), topMargin() + bottomMargin());
	int wCount = 0;
	for (std::vector<short>::const_iterator it = widths.begin(); it != widths.end(); ++it) {
		minSize.Width += *it;
		if (*it != 0) {
			++wCount;
		}
	}
	int hCount = 0;
	for (std::vector<short>::const_iterator it = heights.begin(); it != heights.end(); ++it) {
		minSize.Height += *it;
		if (*it != 0) {
			++hCount;
		}
	}

	if ((wCount == 0) || (hCount == 0)) {
		return;
	}

	minSize.Width += myHorizontalSpacing * (wCount - 1);
	minSize.Height += myVerticalSpacing * (hCount - 1);

	const short deltaW = (size.Width - minSize.Width) / wCount;
	for (std::vector<short>::iterator it = widths.begin(); it != widths.end(); ++it) {
		if (*it != 0) {
			*it += deltaW;
		}
	}
	const short deltaH = (size.Height - minSize.Height) / hCount;
	for (std::vector<short>::iterator it = heights.begin(); it != heights.end(); ++it) {
		if (*it != 0) {
			*it += deltaH;
		}
	}
	lefts.reserve(widths.size());
	short current = leftMargin();
	for (std::vector<short>::iterator it = widths.begin(); it != widths.end(); ++it) {
		lefts.push_back(current);
		if (*it > 0) {
			current += *it + myHorizontalSpacing;
		}
	}


	/*
	std::cerr << "widths: ";
	for (size_t i = 0; i < widths.size(); ++i) {
		std::cerr << widths[i] << ' ';
	}
	std::cerr << "\n";
	std::cerr << "heights: ";
	for (size_t i = 0; i < heights.size(); ++i) {
		std::cerr << heights[i] << ' ';
	}
	std::cerr << "\n";
	*/

	int ey = y + topMargin();
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		short h = heights[it - myRows.begin()];
		if (h > 0) {
			for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
				if (jt->Widget->isVisible()) {
					jt->Widget->setPosition(lefts[jt->XFrom], ey, Size(widths[jt->XFrom], h));
				}
			}
			ey += h + myVerticalSpacing;
		}
	}
}

void W32Table::init(HWND parent, W32ControlCollection *collection) {
	for (std::vector<RowList>::const_iterator it = myRows.begin(); it != myRows.end(); ++it) {
		for (RowList::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
			jt->Widget->init(parent, collection);
		}
	}
}