#ifndef AUDIO_H
#define AUDIO_H
#include <QObject>
#include <QElapsedTimer>
#include <QSoundEffect>
#include <array>

class Audio : public QObject
{
public:
    enum Sound { Deal, Place, Capture, Victory, Finish, SoundCount };
    explicit Audio(QObject *parent = nullptr);
    void play(Sound sound);
    void setMuted(bool value);
    bool isMuted() const { return muted; }
private:
    std::array<QSoundEffect *, SoundCount> effects{};
    QElapsedTimer dealClock;
    bool muted{false};
};
#endif
