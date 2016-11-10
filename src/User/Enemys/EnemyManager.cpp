#include "EnemyManager.h"

#include <algorithm>

#include "cinder/Rand.h"

#include "EnemySlash.h"
#include "EnemyBullet.h"
#include "EnemyBoss.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

namespace User
{
    using namespace cinder;
    EnemyManager::EnemyManager( const cinder::CameraPersp& camera, std::string const& path )
    {
        JsonTree params( app::loadAsset( path ) );

        for ( auto& obj : params["Enemy"] )
        {
            if ( obj.getValue( ) == "Bullet" )
            {
                Create<EnemyBullet>( Vec3f( randFloat( -2.0F, 2.0F ), 2, randFloat( -2, 1 ) ), camera );
            }
            if ( obj.getValue( ) == "Slash" )
            {
                Create<EnemySlash>( Vec3f( randFloat( -2.0F, 2.0F ), 0, randFloat( -2, 1 ) ), camera );
            }
            if ( obj.getValue( ) == "Boss" )
            {
                Create<EnemyBoss>( Vec3f( randFloat( -2.0F, 2.0F ), 0, randFloat( -2, 1 ) ), camera );
            }
        }
    }

    void EnemyManager::update( cinder::CameraPersp const& camera )
    {
        Each( [ &camera ] ( EnemyBaseRef& enemyRef ) { enemyRef->update( camera ); } );

        EnemyEraser( );

        EnemyBulletsRecovery( );
    }

    void EnemyManager::draw( )
    {
        Each( [ ] ( EnemyBaseRef& enemyRef ) { enemyRef->draw( ); } );
    }

    void EnemyManager::drawUI( )
    {
        Each( [ ] ( EnemyBaseRef& enemyRef ) { enemyRef->drawUI( ); } );
    }

    bool EnemyManager::IsAttack( const cinder::CameraPersp& camera )
    {
        bool isAttack = false;
        Each( [ &isAttack, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true )
                isAttack = true;
        } );
        return isAttack;
    }

    int EnemyManager::PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera )
    {
        int drainMp = 0;
        Each( [ &drainMp, &line_, &camera ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;
            drainMp += enemyRef->Hit( CheckDefLineOfCircle( line_, vec, radius ) );
        } );
        return drainMp;
    }

    int EnemyManager::PlayerSpecialAttackToEnemyDamage( int damage )
    {
        int drainMp = 0;
        Each( [ &drainMp, &damage ] ( EnemyBaseRef& enemyRef )
        {
            drainMp += enemyRef->Damage( damage );
        } );
        return drainMp;
    }

    int EnemyManager::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        Each( [ &damage, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true )
                damage += enemyRef->AttackPoint( );
        } );

        return damage;
    }

    int EnemyManager::EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera )
    {
        int totalDamage = 0;
        Each( [ &totalDamage, &line_, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) )
            {
                Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                float radius = Vec3f( size - vec ).length( ) / 2.0F;
                if ( CheckDefLineOfCircle( line_, vec, radius + 50 ) > 1.0f ) totalDamage += enemyRef->AttackPoint( );
            }
        } );
        return totalDamage;
    }

    EnemyBulletList EnemyManager::BulletsRecovery( )
    {
        auto ret = bulletList;
        bulletList.clear( );
        return ret;
    }

    void EnemyManager::DrawCollisionCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;

            gl::color( ColorA( 1, 1, 1, 0.25F ) );
            gl::drawSolidCircle( vec, radius, radius );
        } );
    }

    bool EnemyManager::IsEmpty( )
    {
        return enemyList.empty( );
    }

    void EnemyManager::Each( std::function<void( EnemyBaseRef& )> function )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), function );
    }

    void EnemyManager::EnemyEraser( )
    {
        auto eraceList = std::remove_if( enemyList.begin( ), enemyList.end( ), [ ] ( EnemyBaseRef& enemyRef ) { return !enemyRef->IsActive( ); } );
        enemyList.erase( eraceList, enemyList.end( ) );
    }

    void EnemyManager::EnemyBulletsRecovery( )
    {
        Each( [ this ] ( EnemyBaseRef& enemyRef )
        {
            // splice��list���m���Ȃ���Ƃ��Ɏg���܂��B
            // �ȉ��̃T�C�g���Q�l�ɂ��܂����B
            // http://d.hatena.ne.jp/pknight/20090814/1250227222
            bulletList.splice( bulletList.end( ), enemyRef->BulletsRecovery( ) );
        } );
    }
}