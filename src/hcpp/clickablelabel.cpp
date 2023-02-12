#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent), row(-1), col(-1)
{
}

ClickableLabel::~ClickableLabel()
{

}

int ClickableLabel::getRow()
{
    return row;
}

int ClickableLabel::getCol()
{
    return col;
}

void ClickableLabel::setRow(int _row)
{
    row = _row;
}

void ClickableLabel::setCol(int _col)
{
    col = _col;
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked(row, col);
}
