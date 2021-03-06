# pragma once

# include "EffectBase.h"

# include "../Utilitys/Yumayo.h"

namespace User
{
    // Effectは画像の左上を基点として表示されます。
    class EffectScore : public EffectBase
    {
        float alpha = 0.0F;
        int score;
        Fonts font;
    public:
        EffectScore( cinder::Vec2f position, int scoreNum );
    protected:
        virtual void Update( ) override;
        virtual void Draw( ) override;
    };
}