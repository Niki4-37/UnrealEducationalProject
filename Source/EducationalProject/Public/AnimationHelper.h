#pragma once

class AnimationHelper
{
public:
    template <typename Y>
    static Y* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;

        const TArray<FAnimNotifyEvent> Notifies = Animation->Notifies;
        for (auto& NotifyEvent : Notifies)
        {
            auto RequiredNotify = Cast<Y>(NotifyEvent.Notify);
            if (RequiredNotify) return RequiredNotify;
        }
        return nullptr;
    }
};

