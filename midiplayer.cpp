#include "./midiplayer.h"

using namespace cms;

MidiPlayer::MidiPlayer() {}

MidiPlayer::~MidiPlayer() {
    if (midiin != nullptr) {
        delete midiin;
    }
    if (midiout != nullptr) {
        delete midiout;
    }
}

void MidiPlayer::openPorts() {
    midiin = new RtMidiIn();
    midiout = new RtMidiOut();
}

void MidiPlayer::getPortInfo(std::vector<std::string> *in,
                             std::vector<std::string> *out) const {
    if (in != nullptr && midiin != nullptr) {
        unsigned int nPorts = midiin->getPortCount();
        for (unsigned int i = 0; i < nPorts; i++) {
            in->push_back(midiin->getPortName(i));
        }
    }
    if (out != nullptr && midiout != nullptr) {
        unsigned int nPorts = midiout->getPortCount();
        for (unsigned int i = 0; i < nPorts; i++) {
            out->push_back(midiout->getPortName(i));
        }
    }
}

void MidiPlayer::setOutputPort(unsigned int portNum) {
    midiout->openPort(portNum);
}

void BackgroundMidiPlayer::setTickOffset(int tick) {
    this->tick_offset = tick;
}

BackgroundMidiPlayer::BackgroundMidiPlayer(MidiPlayer &&mp) {
    this->mp.reset(std::move(&mp));
}

BackgroundMidiPlayer::~BackgroundMidiPlayer()
{
    if (copied_mf != nullptr)
        delete copied_mf;
}

void BackgroundMidiPlayer::setMidiFile(const smf::MidiFile &mf) {
    this->copied_mf = new smf::MidiFile(mf);
    copied_mf->doTimeAnalysis();
    copied_mf->joinTracks();
    copied_mf->absoluteTicks();
}

void BackgroundMidiPlayer::run() {
    int offset = 0;
    if (tick_offset > 0) {
        for (int i=0; i<(*copied_mf)[0].size(); i++) {
            if ((*copied_mf)[0][i].tick > tick_offset) {
                offset = i;
                break;
            }
        }
    }
    for (int e = offset; e < (*copied_mf)[0].size(); e++) {
        if (currentThread()->isInterruptionRequested())
            break;
        smf::MidiEvent *mev = &(*copied_mf)[0][e];
        double delta_real;
        if (e == 0)
            delta_real = mev->seconds;
        else
            delta_real = mev->seconds - (*copied_mf)[0][e - 1].seconds;

        ISLEEP(delta_real * 1000);
        std::vector<unsigned char> vec;
        for (size_t i = 0; i < mev->size(); i++) {
            vec.push_back((*mev)[i]);
        }
        mp->sendMessage(vec);
    }

    return;
}

void MidiPlayer::sendMessage(const std::vector<unsigned char> &message) {
    midiout->sendMessage(&message);
}
