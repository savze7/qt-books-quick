#include "bukurepository.h"
#include "sqlhelper.h"
#include <QSqlQuery>
#include <QSqlError>

BukuRepository::BukuRepository(QObject *parent)
    : QObject{parent}
{

}

QList<BukuData *> BukuRepository::getList(QList<int> ignoredKode, QString textQuery, int kategoriFilter, int penerbitFiter)
{
    QStringList filterList;
    QHash<QString, QVariant> filterBinds;

    if (ignoredKode.length() > 0) {
        filterList.append(QString("Buku.kd_buku NOT IN (%1)").arg(
            SQLHelper::generateArrayBinds(":ignored_kode", ignoredKode, filterBinds)
            ));
    }

    if (textQuery.length() > 0) {
        filterList.append("Buku.judul LIKE :text_query");
        filterBinds[":text_query"] = "%" + textQuery + "%";
    }

    if (kategoriFilter != -1) {
        filterList.append("Buku.kd_kategori = :kategori");
        filterBinds[":kategori"] = kategoriFilter;
    }

    if (penerbitFiter != -1) {
        filterList.append("Buku.kd_penerbit = :penerbit");
        filterBinds[":penerbit"] = penerbitFiter;
    }

    QString queryString = "SELECT"
                  "   Buku.kd_buku,"
                  "   Buku.judul,"
                  "   Buku.penulis,"
                  "   Buku.jumlah_hilang,"
                  "   Buku.tahun_terbit,"
                  "   Kategori.kd_kategori,"
                  "   Kategori.nama_kategori,"
                  "   Penerbit.kd_penerbit,"
                  "   Penerbit.nama_penerbit "
                  "FROM Buku"
                  "   JOIN Kategori ON"
                  "       Buku.kd_kategori = Kategori.kd_kategori "
                  "   JOIN Penerbit ON"
                  "       Penerbit.kd_penerbit = Buku.kd_penerbit ";

    if (filterList.length() > 0)
        queryString.append("WHERE ").append(filterList.join(" AND "));

    QSqlQuery query;
    query.prepare(queryString);
    SQLHelper::applyBindMaps(query, filterBinds);

    if (!query.exec())
        qFatal() << "Cannot query for Buku " << query.lastError().text();


    QList<BukuData*> result;
    while (query.next()) {
        result.append(new BukuData(
            query.value("Buku.kd_buku").toInt(),
            query.value("Buku.judul").toString(),
            query.value("Buku.penulis").toString(),
            query.value("Buku.jumlah_hilang").toInt(),
            query.value("Buku.tahun_terbit").toInt(),
            query.value("Kategori.kd_kategori").toInt(),
            query.value("Kategori.nama_kategori").toString(),
            query.value("Penerbit.kd_penerbit").toInt(),
            query.value("Penerbit.nama_penerbit").toString()
            ));
    }

    return result;
}

BukuData* BukuRepository::getBukuData(int kode)
{
    QSqlQuery query;
    query.prepare("SELECT"
                  "   Buku.kd_buku,"
                  "   Buku.judul,"
                  "   Buku.penulis,"
                  "   Buku.jumlah_hilang,"
                  "   Buku.tahun_terbit,"
                  "   Kategori.kd_kategori,"
                  "   Kategori.nama_kategori,"
                  "   Penerbit.kd_penerbit,"
                  "   Penerbit.nama_penerbit "
                  "FROM Buku"
                  "   JOIN Kategori ON"
                  "       Buku.kd_kategori = Kategori.kd_kategori "
                  "   JOIN Penerbit ON"
                  "       Penerbit.kd_penerbit = Buku.kd_penerbit "
                  "WHERE Buku.kd_buku = :kode");
    query.bindValue(":kode", kode);

    if (!query.exec())
        qFatal() << "Cannot get buku data " << query.lastError().text();

    if (query.next()) {
        return new BukuData(
            query.value("Buku.kd_buku").toInt(),
            query.value("Buku.judul").toString(),
            query.value("Buku.penulis").toString(),
            query.value("Buku.jumlah_hilang").toInt(),
            query.value("Buku.tahun_terbit").toInt(),
            query.value("Kategori.kd_kategori").toInt(),
            query.value("Kategori.nama_kategori").toString(),
            query.value("Penerbit.kd_penerbit").toInt(),
            query.value("Penerbit.nama_penerbit").toString()
            );
    } else {
        return new BukuData();
    }
}

void BukuRepository::addNew(QString judul, QString penulis, int tahunTerbit, int kodeKategori, int kodePenerbit)
{
    QSqlQuery query;
    if (!query.exec("SELECT MAX(CAST(kd_buku AS UNSIGNED)) FROM Buku"))
        qFatal() << "Error get max id " << query.lastError();

    int maxId = -1;
    if (query.next()) {
        maxId = query.value(0).toInt();
    }

    query.prepare("INSERT INTO Buku("
                  " kd_buku,"
                  " judul,"
                  " penulis,"
                  " tahun_terbit,"
                  " kd_kategori,"
                  " kd_penerbit"
                  ") VALUES ("
                  " :kode,"
                  " :judul,"
                  " :penulis,"
                  " :tahun_terbit,"
                  " :kategori,"
                  " :penerbit"
                  ")");

    query.bindValue(":kode", QString::number(maxId + 1).rightJustified(4, '0'));
    query.bindValue(":judul", judul);
    query.bindValue(":penulis", penulis);
    query.bindValue(":tahun_terbit", tahunTerbit);
    query.bindValue(":kategori", kodeKategori);
    query.bindValue(":penerbit", kodePenerbit);

    if (!query.exec())
        qFatal() << "Cannot add Buku " << query.lastError().text();

    emit dataChanged();
}

void BukuRepository::edit(int kode, QString judul, QString penulis, int jumlahHilang, int tahunTerbit, int kodeKategori, int kodePenerbit)
{
    QSqlQuery query;
    query.prepare("UPDATE Buku SET "
                  "judul = :judul,"
                  "penulis = :penulis,"
                  "jumlah_hilang = :jumlah,"
                  "tahun_terbit = :tahun_terbit,"
                  "kd_kategori = :kategori,"
                  "kd_penerbit = :penerbit "
                  "WHERE kd_buku = :kode");
    query.bindValue(":kode", kode);
    query.bindValue(":judul", judul);
    query.bindValue(":penulis", penulis);
    query.bindValue(":jumlah", jumlahHilang);
    query.bindValue(":tahun_terbit", tahunTerbit);
    query.bindValue(":kategori", kodeKategori);
    query.bindValue(":penerbit", kodePenerbit);

    if (!query.exec())
        qFatal() << "Cannot edit Buku " << query.lastError().text();

    emit dataChanged();
}

void BukuRepository::remove(int kode)
{
    QSqlQuery query;

    query.prepare("DELETE from Buku WHERE kd_buku = :kode");
    query.bindValue(":kode", kode);
    if (!query.exec())
        qFatal() << "Cannot remove Buku " << query.lastError().text();

    emit dataChanged();
}

bool BukuRepository::isBookAvailable(QList<int> ignoredKode)
{
    QString queryString = "SELECT "
                          " COUNT(kd_buku) > 0 "
                          "FROM Buku ";
    QHash<QString, QVariant> binds;

    if (ignoredKode.length() > 0) {
        queryString += QStringLiteral(" WHERE kd_buku NOT IN(%1)").arg(
            SQLHelper::generateArrayBinds(":ignored_kode", ignoredKode, binds)
            );
    }

    QSqlQuery query;
    query.prepare(queryString);
    SQLHelper::applyBindMaps(query, binds);

    if (!query.exec())
        qFatal() << "Cannot get is buku available " << query.lastError().text();


    return query.next() ? query.value(0).toBool() : false;
}
