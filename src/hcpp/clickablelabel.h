#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = 0);
    ~ClickableLabel();

    int getRow();
    int getCol();
    void setRow(int _row);
    void setCol(int _col);

signals:
    void clicked(int row, int col);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    int row;
    int col;
};

#endif // CLICKABLELABEL_H
