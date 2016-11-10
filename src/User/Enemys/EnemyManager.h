#pragma once

#include <list>
#include <memory>
#include <functional>

#include "cinder/Camera.h"
#include "EnemyBase.h"
#include "../Utilitys/Nomoto.h"

namespace User
{
    using EnemyBaseRef = std::shared_ptr<EnemyBase>;
    using EnemyList = std::list<EnemyBaseRef>;

    class EnemyManager
    {
        EnemyList enemyList;
        EnemyBulletList bulletList;
    public:
        EnemyManager( const cinder::CameraPersp& camera, std::string const& path );
        void update( cinder::CameraPersp const& camera );
        void draw( );
        void drawUI( );
    public:
        // �G�l�~�[�̍쐬���s���܂��B
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera );
        // �G�l�~�[����̂ł��U�����Ă����� true �ɂȂ�܂��B
        bool IsAttack( const cinder::CameraPersp& camera );
        // �v���C���[����G�l�~�[�ւ̃_���[�W���C���Ƃ̓����蔻�������
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // �v���C���[�̕K�E�Z���G�l�~�[�ւƗ^���܂��B
        int PlayerSpecialAttackToEnemyDamage( int damage );
        // �G�l�~�[����v���C���[�ւ̍U���i���ڃ_���[�W��ʂ��܂��j
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // �G�l�~�[����v���C���[�ւ̍U���i�v���C���[���K�[�h�����Ă��鎞�̔���Ɏg���܂��j
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
        // �G�l�~�[�̓����蔻����`�悵�܂��B
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
        bool IsEmpty( );
    private:
        void Each( std::function<void( EnemyBaseRef& )> function );
        // �G�l�~�[��HP���O��������폜
        void EnemyEraser( );
        // �e�G�l�~�[�̒e��������܂��B
        void EnemyBulletsRecovery( );
    };

    using EnemyManagerRef = std::shared_ptr<EnemyManager>;

    template<class Ty>
    inline void EnemyManager::Create( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }
}