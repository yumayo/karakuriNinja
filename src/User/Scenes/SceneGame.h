# pragma once

# include "Scene.hpp"

//Yumayo's include
# include "cinder/Camera.h"
# include "../Enemys/EnemyManager.h"
# include "../Enemys/EnemyBulletManager.h"
# include "../Field/FieldManager.h"
# include "../Effect/EffectManager.h"
# include "../Interfaces/Interface.h"
# include "../Utilitys/Yumayo.h"
# include "../Interfaces/Talk.h"
# include "../Interfaces/Description.h"
# include "../Utilitys/HundAnimation.h"
# include "../Utilitys/MojiManager.h"
# include "cinder/params/Params.h"
# include "../Utilitys/CameraData.h"

//Nomoto's include
# include "../Player/Player.h"

//Hirasawa's include
# include "../Utilitys/Audio.h"
# include "../Special/Special.h"

namespace User
{
    class Fusuma;

    class SceneGame : public SceneBase
    {
    public:
        SceneGame( );
        virtual ~SceneGame( );
    public:
        virtual void resize( ) override;
        virtual void draw( ) override;
        virtual void update( ) override;
        virtual void select( ) override;
    private:
        void beginDrawMain( );
        void drawMain( );
        void endDrawMain( );
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        void UpdateGameStart( );
        void UpdateDamage( );
        void UpdateColor( );
        void UpdateScore( );
        void UpdateCombo( );
        void UpdateDamageExpression( );
        void UpdateNextStage( );
        void UpdateAllInstans( );
        void UpdateSpecial( );
        void UpdateGameEnd( );
    private:
        //=======================================
        // ユーマヨが管理するもののインスタンス化。
        CameraData camera;
        EnemyManagerRef enemyManager;
        EnemyBulletManagerRef enemyBulletManager;
        FieldManagerRef fieldManager;
        EffectManagerRef effectManager;
        InterfaceRef UI;
        Timer timer;
        int maxSceneChangeFrame = 60 * 3;
        int sceneChangeFrame = maxSceneChangeFrame;
        cinder::ColorA damageColor;
        TalkRef talk;
        MoveInput moveInput;
        MojiManager mojiManager;
        //=======================================


        //=======================================
        // 野本が管理するもののインスタンス化。

        Player player;
        //=======================================


        //=======================================
        // 大ちゃんが管理するもののインスタンス化。

        Special special;
        std::vector<Audio*> mainbgm;
        int watercount;
        std::shared_ptr<Fusuma> fusuma;
        int time;
        Audio* mpmax;
        bool ismpmax;
        Audio* horagai;
        //=======================================
    };
}