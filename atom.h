//
// Created by mkh on 19.10.2020.
//

#ifndef DEBUG_MP4_ATOM_H
#define DEBUG_MP4_ATOM_H

#include <istream>
#include <map>
#include <memory>

class TrackFragmentRunBox;
using TrunMap = class std::map<unsigned int, std::shared_ptr<TrackFragmentRunBox> >;

class Atom {
public:
    enum Value {
        nval = 0x00000000,
        ftyp = 0x70797466,
        free = 0x65657266,
        moov = 0x766f6f6d,
        dref = 0x66657264,
        url  = 0x206c7275,
        urn  = 0x206e7275,
        edts = 0x73746465,
        elst = 0x74736c65,
        mdat = 0x7461646d,
        mvhd = 0x6468766d,
        trak = 0x6b617274,
        tkhd = 0x64686b74,
        tref = 0x66657274,
        mdia = 0x6169646d,
        mdhd = 0x6468646d,
        hdlr = 0x726c6468,
        minf = 0x666e696d,
        vmhd = 0x64686d76,
        smhd = 0x64686d73,
        hmhd = 0x64686d68,
        nmhd = 0x64686d6e,
        dinf = 0x666e6964,
        stbl = 0x6c627473,
        stts = 0x73747473,
        ctts = 0x73747463,
        stsd = 0x64737473,
        avc1 = 0x31637661,
        avcc = 0x43637661,
        hev1 = 0x31766568,
        hvcc = 0x43637668,
        mp4a = 0x6134706d,
        esds = 0x73647365,
        stsz = 0x7a737473,
        stsc = 0x63737473,
        mehd = 0x6468656d,
        stco = 0x6f637473,
        co64 = 0x34366f63,
        mvex = 0x7865766d,
        trex = 0x78657274,
        uuid = 0x64697575,
        moof = 0x666f6f6d,
        mfhd = 0x6468666d,
        traf = 0x66617274,
        tfhd = 0x64686674,
        trun = 0x6e757274,
        udta = 0x61746475,
        iods = 0x73646f69
    };

    static Atom * make( std::istream & is, const TrunMap & trunMap );

    Atom( Value val );
    Atom( std::istream & is );
    virtual ~Atom () = default;

    operator uint32_t() const;

    const char * str() const;

    uint64_t size() const;
    std::streampos position() const;
    bool container() const;

    void setIndent( int indent );
    void indent( std::ostream& out ) const;

protected:
    virtual void fout( std::ostream& out ) const;

    void ctime( std::ostream & out, time_t t ) const;

private:
    union {
        Value a;
        char buf[sizeof(uint32_t)];
    } m_type;
    std::string m_strtype;
    uint64_t m_size;
    std::streampos m_position;
    int m_indent {0};

    friend std::ostream & operator <<( std::ostream& out, const Atom& atom );
};

#endif //DEBUG_MP4_ATOM_H
