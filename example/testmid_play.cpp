#include "../midiplayer.h"

#include "../ext/midifile/include/MidiFile.h"


int main()
{
    smf::MidiFile midifile("./test.mid");

    cms::MidiPlayer *midiplayer = new cms::MidiPlayer();
    midiplayer->openPorts();
    std::vector<std::string> outs;
    midiplayer->getPortInfo(nullptr, &outs);
    for (auto out : outs) {
        std::cout << out << std::endl;
    }

    midiplayer->setOutputPort(0);
    auto bmp = new cms::BackgroundMidiPlayer(std::move(*midiplayer));
    midiplayer = nullptr;

    bmp->setMidiFile(midifile);
    bmp->setTickOffset(0);

    bmp->start();
    bmp->wait();

    return 0;
}
