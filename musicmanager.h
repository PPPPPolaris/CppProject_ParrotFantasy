#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class MusicManager
{
public:
    static MusicManager& instance();
    void playMusic(const QString& filePath);
    void stopMusic();
private:
    QMediaPlayer* player = nullptr;
    QAudioOutput* audioOutPut = nullptr;
};

#endif // MUSICMANAGER_H
