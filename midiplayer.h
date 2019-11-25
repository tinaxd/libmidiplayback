#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <memory>
#include <vector>

#include "ext/midifile/include/MidiFile.h"
#include "ext/rtmidi/RtMidi.h"

#define CLASS_DLL_EXPORT class

#if _WIN32
    #include <Windows.h>
    #define ISLEEP(ms) do {Sleep(static_cast<DWORD>(ms));} while (false)
    #undef CLASS_DLL_EXPORT
    #define CLASS_DLL_EXPORT class __declspec(dllexport)
#elif __linux__
    #include <time.h>
    #define ISLEEP(ms) ({const struct timespec ts {0, (ms)*1000000}; nanosleep(&ts, NULL);})
#endif

#include <QThread>
#include <QObject>

namespace cms {

CLASS_DLL_EXPORT MidiPlayer : public QObject{
    Q_OBJECT

public:
    MidiPlayer();
    virtual ~MidiPlayer();

    /*! Throws RtMidiError on error */
    void openPorts();
    void getPortInfo(std::vector<std::string> *in,
                     std::vector<std::string> *out) const;

    void setOutputPort(unsigned int portNum);

    void sendMessage(const std::vector<unsigned char> &message);

private:
    RtMidiIn *midiin;
    RtMidiOut *midiout;
};

CLASS_DLL_EXPORT BackgroundMidiPlayer : public QThread {
    Q_OBJECT

public:
    BackgroundMidiPlayer(MidiPlayer &&mp);
    virtual ~BackgroundMidiPlayer() override;

    void setMidiFile(const smf::MidiFile &mf);
    void setTickOffset(int tick);

private:
    std::unique_ptr<MidiPlayer> mp;
    smf::MidiFile *copied_mf;
    int tick_offset;

    void run() override;
};

} // namespace cms

#endif // MIDIPLAYER_H
