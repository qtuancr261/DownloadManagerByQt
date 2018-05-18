#include "measurementunit.h"

MeasurementUnit::MeasurementUnit()
{

}

QString MeasurementUnit::getConvenientUnitFromBytes(qint64 bytes)
{
    for (int index{}; index < units.size(); index++)
    {
        qreal devidedBytes{qPow(2, index * 10)};
        if (bytes < static_cast<qint64>(qPow(2, (index + 1) * 10)))
            return QString("%1 %2").arg(QString::number(bytes/devidedBytes, 'f', 2), units.at(index));
    }
}
