#ifndef MEASUREMENTUNIT_H
#define MEASUREMENTUNIT_H
#include <QtMath>
#include <QtDebug>

class MeasurementUnit
{
private:
    MeasurementUnit();
public:
    static QVector<QString> units;
    static QString getConvenientUnitFromBytes(qint64 bytes);
};

#endif // MEASUREMENTUNIT_H
