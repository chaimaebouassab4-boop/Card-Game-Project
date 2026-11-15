#include "audio.h"
#include <QSettings>
#include <QUrl>

Audio::Audio(QObject *parent) : QObject(parent)
{
    const QStringList names{"deal", "place", "capture", "victory", "finish"};
    const std::array<qreal, SoundCount> volumes{0.30, 0.40, 0.40, 0.45, 0.35};
    muted = QSettings("Ronda", "Ronda").value("soundMuted", false).toBool();
    for (int i = 0; i < SoundCount; ++i) {
        effects[i] = new QSoundEffect(this);
        effects[i]->setSource(QUrl("qrc:/sounds/" + names[i] + ".wav"));
        effects[i]->setVolume(volumes[i]);
        effects[i]->setMuted(muted);
    }
}
void Audio::play(Sound sound)
{
    if (muted || effects[sound]->status() != QSoundEffect::Ready)
        return;
    if (sound == Deal) {
        if (dealClock.isValid() && dealClock.elapsed() < 65)
            return;
        dealClock.start();
    }
    effects[sound]->play();
}
void Audio::setMuted(bool value)
{
    muted = value;
    for (auto *effect : effects) {
        effect->setMuted(muted);
        if (muted)
            effect->stop();
    }
    QSettings("Ronda", "Ronda").setValue("soundMuted", muted);
}
