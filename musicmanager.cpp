#include "musicmanager.h"
MusicManager &MusicManager::instance()
{
    static MusicManager musicManager;
    return musicManager;
}
void MusicManager::playMusic(const QString& filePath){
    if(!player){
        player = new QMediaPlayer;
        audioOutPut = new QAudioOutput;
        player->setAudioOutput(audioOutPut);
        audioOutPut->setVolume(0.5);
        player->setSource(QUrl(filePath));

        player->setLoops(-1);
    }
    if(!player->isPlaying()){
        player->play();
    }

}

void MusicManager::stopMusic()
{
    if(player && player->isPlaying()){
        player->stop();
    }
}

