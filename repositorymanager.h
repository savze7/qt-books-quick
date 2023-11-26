#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

class KategoriRepository;
class PenerbitRepository;
class BukuRepository;
class PengadaanRepository;
class PeminjamanRepository;
class MemberRepository;

class RepositoryManager
{
private:
    RepositoryManager();

    KategoriRepository* mKategori = nullptr;
    PenerbitRepository* mPenerbit = nullptr;
    BukuRepository* mBuku = nullptr;
    PengadaanRepository* mPengadaan = nullptr;
    PeminjamanRepository* mPeminjaman = nullptr;
    MemberRepository* mMember = nullptr;
public:
    static RepositoryManager *getInstance();

    KategoriRepository* getKategori();
    PenerbitRepository* getPenerbit();
    BukuRepository* getBuku();
    PengadaanRepository* getPengadaan();
    PeminjamanRepository* getPeminjaman();
    MemberRepository* getMember();
};

#endif // REPOSITORYMANAGER_H
