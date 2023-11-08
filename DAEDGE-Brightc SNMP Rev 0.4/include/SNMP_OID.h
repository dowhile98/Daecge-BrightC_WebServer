/**
 * @file SNMP_OID.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __SNMP_OID_H
#define __SNMP_OID_H

#include <Arduino.h>
#include <SNMP.h>
#include "main.h"
/*Defines ----------------------------------------------------------------*/
#define BRIGHTC_ENTERPRESISE        (const char *)"1.3.6.1.4.1.3134.2.5.7"
#define BRIGHTC_TRAPSDESCRIP        (const char *)"1.3.6.1.4.1.3134.2.5.6.1"
#define BRIGHTC_TRAPSEVERITY        (const char *)"1.3.6.1.4.1.3134.2.5.6.2"

#define TRAP_SEVERITY_WARNING       1
#define TRAP_SEVERITY_MINOR         2
#define TRAP_SEVERITY_MAJOR         3
#define TRAP_SEVERITY_CRITICAL      4

typedef enum{
    TRAP_SYSTEMUP       = 0,
    TRAP_AS1,
    TRAP_AAPB1,
    TRAP_AS2,
    TRAP_AAPB2,
    TRAP_AS3,
    TRAP_AAPB3,
    TRAP_AS4,
    TRAP_AAPB4,
    TRAP_TEMPHIGH,
    TRAP_BYPASS,
    TRAP_MANUAL,
    TRAP_AA1,
    TRAP_AA2,
    TRAP_AA3,
    TRAP_AA4,
    TRAP_TURNCHANGE,
    TRAP_INCENDIO,
    TRAP_COLDSTART,
    TRAP_WARNSTART
}BrigthC_Traps_t;
//OID DE SET Y GET
#define NAME_OID                (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.0"
#define ID_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.1"
#define IP_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.2"
#define T_MAX_OID               (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.3"
#define T_MIN_OID               (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.4"
#define T_ALARMA_OID            (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.5"    
#define T_BYPASS_OID            (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.6"
#define SEC_DIAS_OID            (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.7"
#define CANT_AA_OID             (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.8"
#define MAXTIME_COOL_SP_OID     (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.9"
#define SAMPLE_RATE_OID         (const char*)"1.3.6.1.4.1.3134.2.5.1.1.1.10"


#define RST_ENERGY_OID          (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.1"
#define MANUAL_OID              (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.2"
#define BYPASS_LOGICO_OID       (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.3"
#define SET_MANUAL_AA1_OID      (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.4"
#define SET_MANUAL_AA2_OID      (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.5"
#define SET_MANUAL_AA3_OID      (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.6"
#define SET_MANUAL_AA4_OID      (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.7"
#define RST_FAIL_COUNT_OID      (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.8"
#define RST_DEVICE_OID          (const char*)"1.3.6.1.4.1.3134.2.5.2.1.1.9"
//SOLO GET(const char*)
#define HUM_OID                 (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.1"
#define TEMP1_OID               (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.2"
#define TEMP2_OID               (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.3"
#define RMS_V_OID               (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.4"
#define PA_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.5"
#define PR_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.6"
#define PAP_OID                 (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.7"
#define EA_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.8"
#define ER_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.9"
#define FP_OID                  (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.10"
#define FREC_OID                (const char*)"1.3.6.1.4.1.3134.2.5.3.1.1.11"


#define AAA1_AS_OID             (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.1"
#define AAA1_AABP_OID           (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.2"
#define AAA2_AS_OID             (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.3"
#define AAA2_AABP_OID           (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.4"
#define AAA3_AS_OID             (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.5"
#define AAA3_AABP_OID           (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.6"
#define AAA4_AS_OID             (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.7"
#define AAA4_AABP_OID           (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.8"
#define S_BYPASS_US_OID         (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.9"
#define A_TMP_HIGH_OID          (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.10"
#define A_FIRE_OID              (const char*)"1.3.6.1.4.1.3134.2.5.5.1.1.11"


#define AA1_TURN_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.1"
#define AA1_STATE_OID           (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.2"
#define AA1_HELP_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.3"
#define AA1_FAIL_COUNT_OID      (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.4"
#define AA2_TURN_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.5"
#define AA2_STATE_OID           (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.6"
#define AA2_HELP_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.7"
#define AA2_FAIL_COUNT_OID      (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.8"
#define AA3_TURN_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.9"
#define AA3_STATE_OID           (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.10"
#define AA3_HELP_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.11"
#define AA3_FAIL_COUNT_OID      (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.12"
#define AA4_TURN_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.13"
#define AA4_STATE_OID           (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.14"
#define AA4_HELP_OID            (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.15"
#define AA4_FAIL_COUNT_OID      (const char*)"1.3.6.1.4.1.3134.2.5.4.1.1.16"
/*Class definition ------------------------------------------------------*/

using SNMP::IntegerBER;
using SNMP::ObjectIdentifierBER;
using SNMP::OctetStringBER;
using SNMP::TimeTicksBER;
using SNMP::VarBind;
using SNMP::VarBindList;
using SNMP::BooleanBER;
using SNMP::OpaqueBER;
using SNMP::OpaqueFloatBER;
using SNMP::BER;
// This class encapsulates the MIB objects interface
class MIB {
    using Setter = void (MIB::*)(const char*);

    // Simple helper class to handle communities
    class Community {
    public:
        static bool isKnown(const char *name) {
            return match(name) != UNKNOWN;
        }

        static bool isReadWrite(const char *name) {
            return match(name) == READWRITE;
        }

    private:
        enum {
            READONLY,
            READWRITE,
            UNKNOWN,
            COUNT = UNKNOWN,
        };

        static inline const char *COMUNITIES[] = {
                "public",
                "private",
        };

        // Returns index of community equals to name
        // Returns UNKNOWN if none
        static unsigned int match(const char *name) {
            for (unsigned int index = 0; index < COUNT; ++index) {
                if (strcmp(COMUNITIES[index], name) == 0) {
                    return index;
                }
            }
            return UNKNOWN;
        }
    };

public:
    // Simple helper class to handle OID
    class OID {
    public:
        enum {
            SYSDESCR            ,           // Read only
            SYSOBJECTID         ,           // Read only
            SYSUPTIME           ,           // Read only
            SYSCONTACT          ,           // Read write
            SYSNAME             ,           // Read write
            SYSLOCATION         ,           // Read write
            //GET/SET (Lectura/Escritura)
            //->OctetString
            NAME                ,           // read write (OctecString)
            ID                  ,                 
            IP                  ,
            T_MAX               ,
            T_MIN               ,
            T_ALARMA            ,    
            T_BYPASS            ,
            //->Integer
            SEC_DIAS            ,
            CANT_AA             ,
            MAXTIME_COOL_SP     ,
            SAMPLE_RATE         ,
            //->bool
            RST_ENERGY          ,
            MANUAL              ,
            BYPASS_LOGICO       ,
            SET_MANUAL_AA1   ,
            SET_MANUAL_AA2   ,
            SET_MANUAL_AA3   ,
            SET_MANUAL_AA4   ,
            RST_FAIL_COUNT      ,
            RST_DEVICE      ,
            //GET(Lectura)
            //->OctetString
            HUM                 ,
            TEMP1               ,
            TEMP2               ,
            RMS_V               ,
            PA                  ,
            PR                  ,
            PAP                 ,
            EA                  ,
            ER                  ,
            FP                  ,
            //->Integer
            FREC                ,
            //Alarmas
            //bool
            AAA1_AS             ,
            AAA1_AABP           ,
            AAA2_AS             ,
            AAA2_AABP           ,
            AAA3_AS             ,
            AAA3_AABP           ,
            AAA4_AS             ,
            AAA4_AABP           ,
            S_BYPASS_US         ,
            A_TMP_HIGH          ,
            A_FIRE              ,
            AA1_TURN            ,
            AA1_STATE           ,
            AA1_HELP            ,
            AA1_FAIL_COUNT      ,   //octec string
            AA2_TURN            ,
            AA2_STATE           ,
            AA2_HELP            ,   //octecstring
            AA2_FAIL_COUNT      ,
            AA3_TURN            ,
            AA3_STATE           ,
            AA3_HELP            ,
            AA3_FAIL_COUNT      ,   //octetstring
            AA4_TURN            ,
            AA4_STATE           ,
            AA4_HELP            ,
            AA4_FAIL_COUNT      ,   //octetstring         
            LAST = AA4_FAIL_COUNT,          // Used for GETNEXTREQUEST
            COUNT,
        };

        static inline const char *NAMES[] = {
                (const char *)"1.3.6.1.2.1.1.1.0",
                (const char *)"1.3.6.1.2.1.1.2.0",
                (const char *)"1.3.6.1.2.1.1.3.0",
                (const char *)"1.3.6.1.2.1.1.4.0",
                (const char *)"1.3.6.1.2.1.1.5.0",
                (const char *)"1.3.6.1.2.1.1.6.0",
                NAME_OID             ,
                ID_OID               ,
                IP_OID               ,
                T_MAX_OID            ,
                T_MIN_OID            ,
                T_ALARMA_OID         ,   
                T_BYPASS_OID         ,
                SEC_DIAS_OID         ,
                CANT_AA_OID          ,
                MAXTIME_COOL_SP_OID  ,
                SAMPLE_RATE_OID       ,
                RST_ENERGY_OID        ,
                MANUAL_OID            ,
                BYPASS_LOGICO_OID     ,
                SET_MANUAL_AA1_OID ,
                SET_MANUAL_AA2_OID ,
                SET_MANUAL_AA3_OID ,
                SET_MANUAL_AA4_OID ,
                RST_FAIL_COUNT_OID    ,
                RST_DEVICE_OID,
                HUM_OID                ,
                TEMP1_OID              ,
                TEMP2_OID              ,
                RMS_V_OID              ,
                PA_OID                 ,
                PR_OID                 ,
                PAP_OID                ,
                EA_OID                 ,
                ER_OID                 ,
                FP_OID                 ,
                FREC_OID               ,
                AAA1_AS_OID            ,
                AAA1_AABP_OID          ,
                AAA2_AS_OID            ,
                AAA2_AABP_OID          ,
                AAA3_AS_OID            ,
                AAA3_AABP_OID          ,
                AAA4_AS_OID            ,
                AAA4_AABP_OID          ,
                S_BYPASS_US_OID        ,
                A_TMP_HIGH_OID         ,
                A_FIRE_OID             ,
                AA1_TURN_OID           ,
                AA1_STATE_OID          ,
                AA1_HELP_OID           ,
                AA1_FAIL_COUNT_OID     ,
                AA2_TURN_OID           ,
                AA2_STATE_OID          ,
                AA2_HELP_OID           ,
                AA2_FAIL_COUNT_OID     ,
                AA3_TURN_OID           ,
                AA3_STATE_OID          ,
                AA3_HELP_OID           ,
                AA3_FAIL_COUNT_OID     ,
                AA4_TURN_OID           ,
                AA4_STATE_OID          ,
                AA4_HELP_OID           ,
                AA4_FAIL_COUNT_OID     

        };

        // Returns index of OID equals to name
        // Returns COUNT if none
        static unsigned int match(const char *name) {
            for (unsigned int index = 0; index < COUNT; ++index) {
                if (strcmp(NAMES[index], name) == 0) {
                    return index;
                }
            }
            return COUNT;
        }

        // Returns index of first OID starting with name
        // Returns COUNT if none
        static unsigned int start(const char *name) {
            for (unsigned int index = 0; index < COUNT; ++index) {
                if (strncmp(NAMES[index], name, strlen(name)) == 0) {
                    return index;
                }
            }
            return COUNT;
        }
    };

    // Process incoming message
    bool message(const SNMP::Message *request, SNMP::Message *response) {
        switch (request->getType()) {
        case SNMP::TYPE_GETREQUEST:
            return get(request, response);
        case SNMP::TYPE_GETNEXTREQUEST:
            return next(request, response);
        case SNMP::TYPE_SETREQUEST:
            return set(request, response);
        }
        return false;
    }

    // Build trap message
    SNMP::Message* trap(IPAddress agentIp, const char * dscrp, uint8_t severity, const uint8_t type) {
        SERIAL_PRINT("[TRAP]->TRAP TYPE:");
        SERIAL_PRINT(type);
        SERIAL_PRINT(" DSCP: ");
        SERIAL_PRINTLN(dscrp);
        SNMP::Message* message = nullptr;
        message = new SNMP::Message(SNMP::VERSION1, "public", SNMP::TYPE_TRAP);
        // Set enterprise OID
        message->setEnterprise(BRIGHTC_ENTERPRESISE);
        // Set agent address
        message->setAgentAddress(agentIp);
        switch (type) {
            case TRAP_SYSTEMUP:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(OID::NAMES[OID::SYSLOCATION], new OctetStringBER(_location));
                message->add(OID::NAMES[OID::SYSCONTACT], new OctetStringBER(_contact));
                break;
            case TRAP_AS1:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA1_AS_OID, new IntegerBER(_aaa1_as));
                break;
            case TRAP_AAPB1:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA1_AABP_OID, new IntegerBER(_aaa1_aabp));
                break;
            case TRAP_AS2:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA2_AS_OID, new IntegerBER(_aaa2_as));
                break;
            case TRAP_AAPB2:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA2_AABP_OID, new IntegerBER(_aaa2_aabp));
                break;
            case TRAP_AS3:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA3_AS_OID, new IntegerBER(_aaa3_as));
                break;
            case TRAP_AAPB3:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA3_AABP_OID, new IntegerBER(_aaa3_aabp));
                break;
            case TRAP_AS4:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA4_AS_OID, new IntegerBER(_aaa4_as));
                break;
            case TRAP_AAPB4:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AAA4_AABP_OID, new IntegerBER(_aaa4_aabp));
                break;
            case TRAP_TEMPHIGH:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(A_TMP_HIGH_OID, new IntegerBER(_a_tmp_high));
                break;
            case TRAP_BYPASS:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(S_BYPASS_US_OID, new IntegerBER(_s_bypass_us));
                break;
            case TRAP_MANUAL:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                break;
            case TRAP_AA1:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AA1_STATE_OID, new IntegerBER(_aa1_state));
                break;
            case TRAP_AA2:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AA2_STATE_OID, new IntegerBER(_aa2_state));
                break;
            case TRAP_AA3:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AA3_STATE_OID, new IntegerBER(_aa3_state));
                break;
            case TRAP_AA4:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                //message->add(AA4_STATE_OID, new IntegerBER(_aa4_state));
                break;  
            case TRAP_TURNCHANGE:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                break;
            case TRAP_INCENDIO:
                message->setTrap(SNMP::ENTERPRISE_SPECIFIC, type);
                message->add(BRIGHTC_TRAPSDESCRIP, new OctetStringBER(dscrp));
                message->add(BRIGHTC_TRAPSEVERITY, new IntegerBER(severity));
                break;
            case TRAP_COLDSTART:
                message->setTrap(SNMP::COLD_START);
                break;
            case TRAP_WARNSTART:
                message->setTrap(SNMP::WARM_START);
                break;
        }
        return message;
    }
    // Setters
    void setContact(const char* contact) {
        free(_contact);
        _contact = strdup((const char*)contact);
    }

    void setLocation(const char* location) {
        free(_location);
        _location = strdup((const char*)location);
    }

    void setName(const char* name) {
        free(_name);
         _name = strdup((const char*)name);
      }
    void setID(const char* id){
        free(_id);
        _id = strdup((const char*)id);
    }
    void setIP(const char* ip){
        free(_ip);
        _ip = strdup((const char*)ip);
    }
    void setT_Max(const char* tmax){
        free(_t_max);
        _t_max = strdup((const char*)tmax);
    }
    void setT_Min(const char* tmin){
        free(_t_min);
        _t_min = strdup((const char*)tmin);
    }
    void setT_Alarma(const char* talarma){
        free(_t_alarma);
        _t_alarma = strdup((const char*)talarma);
    }
    void setT_Bypass(const char* tbypass){
        free(_t_bypass);
        _t_bypass = strdup((const char*)tbypass);
    }
    void setSec_Dias(const char*  secdias){
        _sec_dias = *((uint8_t*)secdias);
    }
    void setCant_AA(const char* caa){
        _cant_aa = *((uint8_t*)caa);
    }
    void setMax_Cool_sp(const char*clsp){
        _maxtime_cool_sp = *((uint32_t*)clsp);
    }
    void setSampleTime(const char* samplet){
        _sample_time = *((uint32_t*)samplet);
    }
    void setBypassLogico(const char* bypassl){
        _bypass_logico = *((uint8_t *)bypassl);

    }
    void setManual(const char*  manual){
        _manual = *((uint8_t *)manual);
    }
    void setRstEnergy(const char*  rst){
        _rst_energy = *((uint8_t *)rst);
    }
    void setManualAA1(const char*  manual1){
        _set_manual_aa1 = *((uint8_t *)manual1);
    }
    void setManualAA2(const char*  manual2){
        _set_manual_aa2 = *((uint8_t *)manual2);
    }
    void setManualAA3(const char*  manual3){
        _set_manual_aa3 = *((uint8_t *)manual3);
    }
    void setManualAA4(const char*  manual4){
        _set_manual_aa4 = *((uint8_t *)manual4);
    }
    void setRstFailCount(const char*  rst){
        _rst_fail_count = *((uint8_t *)rst);
    }
    void serRstdevice(const char * rst){
        _rst_device = *((uint8_t*)rst);
    }
    void setInputVal(Input_val_t &xData){
        _aaa1_as = xData.in1;
        _aaa1_aabp = xData.in2;
        _aaa2_as = xData.in3;
        _aaa2_aabp = xData.in4;
        _aaa3_as = xData.in5;
        _aaa3_aabp = xData.in6;
        _aaa4_as = xData.in7;
        _aaa4_aabp = xData.in8;
        _s_bypass_us = xData.S_BYPASSS_STS;
        _a_tmp_high = xData.TMP_High;
        _a_fire = xData.A_Incendio;
    }
    void setMeasurement(Measurements_t &xData){
        //char val[20];
        sprintf(_tmp1, "%f", xData.TMP1);
        //_tmp1 = val;
        sprintf(_tmp2, "%f", xData.TMP2);
        //_tmp2 = val;
        sprintf(_hum, "%f", xData.HUMEDAD);
        //_hum = (char*)String(xData.HUMEDAD).c_str();
        sprintf(_rms_v, "%f", xData.RMS_V);
        //_rms_v = (char*)String(xData.RMS_V).c_str();
        sprintf(_pa, "%f", xData.PA);
        //_pa = (char*)String(xData.PA).c_str();
        sprintf(_pr, "%f", xData.PR);
        //_pr = (char*)String(xData.PR).c_str();
        sprintf(_pap, "%f", xData.PAP);
        //_pap = (char*)String(xData.PAP).c_str();
        sprintf(_ea, "%f", xData.EA);
        //_ea = (char*)String(xData.EA).c_str();
        sprintf(_er, "%f", xData.ER);
        sprintf(_fp, "%f", xData.FP);
        _freq = (uint16_t)xData.FREC;
    }
    void setAAState(AA_t *xData){
        
        _aa1_turn = xData[0].turn;
        _aa1_state = xData[0].workig;
        _aa1_help = xData[0].helper;
        _aa1_failcount = xData[0].failcount;

        _aa2_turn = xData[1].turn;
        _aa2_state = xData[1].workig;
        _aa2_help = xData[1].helper;
        _aa2_failcount = xData[1].failcount;

        _aa3_turn = xData[2].turn;
        _aa3_state = xData[2].workig;
        _aa3_help = xData[2].helper;
        _aa3_failcount = xData[2].failcount;

        _aa4_turn = xData[3].turn;
        _aa4_state = xData[3].workig;
        _aa4_help = xData[3].helper;
        _aa4_failcount = xData[3].failcount;
    }
private:
    // Add a MIB node specified by its index as a variable binding to an SNMP message.
    VarBind* add(SNMP::Message *message, unsigned int index) {
        switch (index) {
        case OID::SYSDESCR:
            return message->add(OID::NAMES[index], new OctetStringBER(_descr));
        case OID::SYSOBJECTID:
            return message->add(OID::NAMES[index], new ObjectIdentifierBER(_objectID));
        case OID::SYSUPTIME:
            return message->add(OID::NAMES[index], new TimeTicksBER(millis() / 10));
        case OID::SYSCONTACT:
            return message->add(OID::NAMES[index], new OctetStringBER(_contact));
        case OID::SYSNAME:
            return message->add(OID::NAMES[index], new OctetStringBER(_name));
        case OID::SYSLOCATION:
            return message->add(OID::NAMES[index], new OctetStringBER(_location));
        case OID::NAME :
            return message->add(OID::NAMES[index], new OctetStringBER(_name));
        case OID::ID :
            return message->add(OID::NAMES[index], new OctetStringBER(_id));
        case OID::IP:
            return message->add(OID::NAMES[index], new OctetStringBER(_ip));
        case OID::T_MAX:
            return message->add(OID::NAMES[index], new OctetStringBER(_t_max));
        case OID::T_MIN:
            return message->add(OID::NAMES[index], new OctetStringBER(_t_min));
        case OID::T_ALARMA:
            return message->add(OID::NAMES[index], new OctetStringBER(_t_alarma));
        case OID::T_BYPASS:
            return message->add(OID::NAMES[index], new OctetStringBER(_t_bypass));
        case OID::SEC_DIAS:
            return message->add(OID::NAMES[index], new IntegerBER(_sec_dias));
        case OID::CANT_AA:
            return message->add(OID::NAMES[index], new IntegerBER(_cant_aa));
        case OID::MAXTIME_COOL_SP:
            return message->add(OID::NAMES[index], new IntegerBER(_maxtime_cool_sp));
        case OID::SAMPLE_RATE:
            return message->add(OID::NAMES[index], new IntegerBER(_sample_time));
        case OID::BYPASS_LOGICO:
            return message->add(OID::NAMES[index], new IntegerBER(_bypass_logico));
        case OID::MANUAL:
            return message->add(OID::NAMES[index], new IntegerBER(_manual));
        case OID::RST_ENERGY:
            return message->add(OID::NAMES[index], new IntegerBER(_rst_energy));
        case OID::SET_MANUAL_AA1:
            return message->add(OID::NAMES[index], new IntegerBER(_set_manual_aa1));
        case OID::SET_MANUAL_AA2:
            return message->add(OID::NAMES[index], new IntegerBER(_set_manual_aa2));
        case OID::SET_MANUAL_AA3:
            return message->add(OID::NAMES[index], new IntegerBER(_set_manual_aa3));
        case OID::SET_MANUAL_AA4:
            return message->add(OID::NAMES[index], new IntegerBER(_set_manual_aa4));
        case OID::TEMP1:
            return message->add(OID::NAMES[index], new OctetStringBER(_tmp1));
        case OID::TEMP2:
            return message->add(OID::NAMES[index], new OctetStringBER(_tmp2));
        case OID::HUM:
            return message->add(OID::NAMES[index], new OctetStringBER(_hum));
        case OID::RMS_V:
            return message->add(OID::NAMES[index], new OctetStringBER(_rms_v));
        case OID::PA:
            return message->add(OID::NAMES[index], new OctetStringBER(_pa));
        case OID::PR:
            return message->add(OID::NAMES[index], new OctetStringBER(_pr));
        case OID::PAP:
            return message->add(OID::NAMES[index], new OctetStringBER(_pap));
        case OID::EA:
            return message->add(OID::NAMES[index], new OctetStringBER(_ea));
        case OID::ER:
            return message->add(OID::NAMES[index], new OctetStringBER(_ea));
        case OID::FP:
            return message->add(OID::NAMES[index], new OctetStringBER(_fp));
        case OID::FREC:
            return message->add(OID::NAMES[index], new IntegerBER(_freq));
        case OID::AAA1_AS:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa1_as));
        case OID::AAA1_AABP:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa1_aabp));
        case OID::AAA2_AS:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa2_as));
        case OID::AAA2_AABP:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa2_aabp));
        case OID::AAA3_AS:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa3_as));
        case OID::AAA3_AABP:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa3_aabp));
        case OID::AAA4_AS:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa4_as));
        case OID::AAA4_AABP:
            return message->add(OID::NAMES[index], new IntegerBER(_aaa4_aabp));
        case OID::S_BYPASS_US:
            return message->add(OID::NAMES[index], new IntegerBER(_s_bypass_us));
        case OID::A_TMP_HIGH:
            return message->add(OID::NAMES[index], new IntegerBER(_a_tmp_high));
        case OID::A_FIRE:
            return message->add(OID::NAMES[index], new IntegerBER(_a_fire));
        case OID::AA1_TURN:
            return message->add(OID::NAMES[index], new IntegerBER(_aa1_turn));
        case OID::AA1_STATE:
            return message->add(OID::NAMES[index], new IntegerBER(_aa1_state));
        case OID::AA1_HELP:
            return message->add(OID::NAMES[index], new IntegerBER(_aa1_help));
        case OID::AA1_FAIL_COUNT:
            return message->add(OID::NAMES[index], new IntegerBER(_aa1_failcount));
        case OID::AA2_TURN:
            return message->add(OID::NAMES[index], new IntegerBER(_aa2_turn));
        case OID::AA2_STATE:
            return message->add(OID::NAMES[index], new IntegerBER(_aa2_state));
        case OID::AA2_HELP:
            return message->add(OID::NAMES[index], new IntegerBER(_aa2_help));
        case OID::AA2_FAIL_COUNT:
            return message->add(OID::NAMES[index], new IntegerBER(_aa2_failcount));
        case OID::AA3_TURN:
            return message->add(OID::NAMES[index], new IntegerBER(_aa3_turn));
        case OID::AA3_STATE:
            return message->add(OID::NAMES[index], new IntegerBER(_aa3_state));
        case OID::AA3_HELP:
            return message->add(OID::NAMES[index], new IntegerBER(_aa3_help));
        case OID::AA3_FAIL_COUNT:
            return message->add(OID::NAMES[index], new IntegerBER(_aa3_failcount));
        case OID::AA4_TURN:
            return message->add(OID::NAMES[index], new IntegerBER(_aa4_turn));
        case OID::AA4_STATE:
            return message->add(OID::NAMES[index], new IntegerBER(_aa4_state));
        case OID::AA4_HELP:
            return message->add(OID::NAMES[index], new IntegerBER(_aa4_help));
        case OID::AA4_FAIL_COUNT:
            return message->add(OID::NAMES[index], new IntegerBER(_aa4_failcount));
        case OID::RST_DEVICE:
            return message->add(OID::NAMES[index], new IntegerBER(_rst_device));
        case OID::RST_FAIL_COUNT:
            return message->add(OID::NAMES[index], new IntegerBER(_rst_fail_count));
        }
        return nullptr;
    }
    
    // Set a MIB node value from a variable binding and add node to response
    void set(SNMP::Message *response, VarBind *varbind, unsigned int match, unsigned int index, MIB::Setter setter) {
        // Check only variable binding value type, should be OCTET STRING
        if (varbind->getValue()->getType() == SNMP::TYPE_OCTETSTRING) {
            // Call setter with value
            (this->*setter)( (const char*)static_cast<OctetStringBER*>(varbind->getValue())->getValue());
            // Add MIB object to response
            add(response, match);
        }else if(varbind->getValue()->getType() == SNMP::TYPE_INTEGER){
            uint32_t v = static_cast<IntegerBER*>((varbind->getValue()))->getValue();
            // Call setter with value
            (this->*setter)((const char* )&v);
            // Add MIB object to response
            add(response, match);

        }else if(varbind->getValue()->getType() == SNMP::TYPE_BOOLEAN){
            bool v = static_cast<BooleanBER*>((varbind->getValue()))->getValue();
            // Call setter with value
            (this->*setter)((const char*)&v);
            // Add MIB object to response
            add(response, match);
        } 
        else {
            // Set error status and index
            response->setError(SNMP::WRONG_TYPE, index + 1);
            // Add OID to response with null value
            response->add(varbind->getName());
        }
    }

    // Process incoming GETREQUEST message and build response
    bool get(const SNMP::Message *message, SNMP::Message *response) {
        // Check if community is known
        if (Community::isKnown(message->getCommunity())) {
            // Get the variable binding list from the message.
            VarBindList *varbindlist = message->getVarBindList();
            for (unsigned int index = 0; index < varbindlist->count(); ++index) {
                // Each variable binding is a sequence of 2 objects:
                // - First one is and ObjectIdentifierBER. It holds the OID
                // - Second is the value of any type
                VarBind *varbind = (*varbindlist)[index];
                // There is a convenient function to get the OID as a const char*
                const char *name = varbind->getName();
                Serial.print("OID:");
                Serial.println(name);
                unsigned int match = OID::match(name);
                switch (match) {
                    
                case OID::COUNT:
                    // OID is unknown
                    switch (message->getVersion()) {
                    case SNMP::VERSION1:
                        // Set error, status and index
                        response->setError(SNMP::NO_SUCH_NAME, index + 1);
                        break;
                    case SNMP::VERSION2C:
                        // No such object
                        response->add(name, new SNMP::NoSuchObjectBER());
                        break;
                    }
                    break;
                default:
                    // OID is identified
                    add(response, match);
                    break;
                }
            }
            return true;
        }
        return false;
    }

    // Process incoming GETNEXTREQUEST message and build response
    bool next(const SNMP::Message *message, SNMP::Message *response) {
        // Check if community is known
        if (Community::isKnown(message->getCommunity())) {
            // Get the variable binding list from the message.
            VarBindList *varbindlist = message->getVarBindList();
            for (unsigned int index = 0; index < varbindlist->count(); ++index) {
                // Each variable binding is a sequence of 2 objects:
                // - First one is and ObjectIdentifierBER. It holds the OID
                // - Second is the value of any type
                VarBind *varbind = (*varbindlist)[index];
                // There is a convenient function to get the OID as a const char*
                const char *name = varbind->getName();
                unsigned int start = OID::start(name);
                Serial.print("NEXT:");
                Serial.println(name);
                switch (start) {
                case OID::LAST:
                    // This is the last OID of the MIB
                    switch (message->getVersion()) {
                    case SNMP::VERSION1:
                        // Set error, status and index
                        response->setError(SNMP::NO_SUCH_NAME, index + 1);
                        // Add OID to response with null value;
                        response->add(name);
                        break;
                    case SNMP::VERSION2C:
                        // End of MIB view
                        response->add(name, new SNMP::EndOfMIBViewBER());
                        break;
                    }
                    break;
                case OID::COUNT:
                    // OID is unknown
                    // Set error, status and index
                    response->setError(SNMP::GEN_ERR, index + 1);
                    // Add OID to response with null value;
                    response->add(name);
                    break;
                default:
                    // Add next object of the MIB
                    add(response, start + 1);
                    break;
                }
            }
            return true;
        }
        return false;
    }

    // Process incoming SETTREQUEST message and build response
    bool set(const SNMP::Message *message, SNMP::Message *response) {
        // Check if community is known
        if (Community::isReadWrite(message->getCommunity())) {
            // Get the variable binding list from the message.
            VarBindList *varbindlist = message->getVarBindList();
            for (unsigned int index = 0; index < varbindlist->count(); ++index) {
                // Each variable binding is a sequence of 2 objects:
                // - First one is and ObjectIdentifierBER. It holds the OID
                // - Second is the value of any type
                VarBind *varbind = (*varbindlist)[index];
                // There is a convenient function to get the OID as a const char*
                const char *name = varbind->getName();
                unsigned int match = OID::match(name);
                Serial.print("SET:");
                Serial.println(name);
                switch (match) {
                case OID::SYSCONTACT:
                    // Set value for MIB node SYSCONTACT
                    // Set error on failure
                    set(response, varbind, match, index, &MIB::setContact);
                    break;
                case OID::SYSNAME:
                    // Set value for MIB node SYSNAME
                    // Set error on failure
                    set(response, varbind, match, index, &MIB::setName);
                    break;
                case OID::SYSLOCATION:
                    // Set value for MIB node SYSLOCATION
                    // Set error on failure
                    set(response, varbind, match, index, &MIB::setLocation);
                    break;
                case OID::NAME:
                    set(response, varbind, match, index, &MIB::setName);
                    dataCmd.command = CMD_NAME;
                    dataCmd.pData = _name;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::ID:
                    set(response, varbind, match, index, &MIB::setID);
                    dataCmd.command = CMD_ID;
                    dataCmd.pData = _id;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::IP:
                    set(response, varbind, match, index, &MIB::setIP);
                    dataCmd.command = CMD_IP;
                    dataCmd.pData = _ip;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::T_MAX:
                    set(response, varbind, match, index, &MIB::setT_Max);
                    dataCmd.command = CMD_T_MAX;
                    dataCmd.pData = _t_max;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::T_MIN:
                    set(response, varbind, match, index, &MIB::setT_Min);
                    dataCmd.command = CMD_T_MIN;
                    dataCmd.pData = _t_min;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::T_ALARMA:
                    set(response, varbind, match, index, &MIB::setT_Alarma);
                    dataCmd.command = CMD_T_ALARMA;
                    dataCmd.pData = _t_alarma;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::T_BYPASS:
                    set(response, varbind, match, index, &MIB::setT_Bypass);
                    dataCmd.command = CMD_T_BP;
                    dataCmd.pData = _t_bypass;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                case OID::SEC_DIAS:
                    set(response, varbind, match, index, &MIB::setSec_Dias);
                    dataCmd.command = CMD_SEC_DIAS;
                    dataCmd.pData = (const char*)&_sec_dias;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::CANT_AA:
                    set(response, varbind, match, index, &MIB::setCant_AA);
                    dataCmd.command = CMD_CANT_AA;
                    dataCmd.pData = (const char*)&_cant_aa;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::MAXTIME_COOL_SP:
                    set(response, varbind, match, index, &MIB::setMax_Cool_sp);
                    dataCmd.command = CMD_MAXTIME_COOL_SP;
                    dataCmd.pData = (const char*)&_maxtime_cool_sp;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::SAMPLE_RATE:
                    set(response, varbind, match, index, &MIB::setSampleTime);
                    dataCmd.command = CMD_SAMPLE_RATE;
                    dataCmd.pData = (const char*)&_sample_time;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::BYPASS_LOGICO:
                    set(response, varbind, match, index, &MIB::setBypassLogico);
                    dataCmd.command = CMD_BYPASS_LOGICO;
                    dataCmd.pData = (const char*)&_bypass_logico;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::MANUAL:
                    set(response, varbind, match, index, &MIB::setManual);
                    dataCmd.command = CMD_MANUAL;
                    dataCmd.pData = (const char*)&_manual;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    if(_manual == false){
                        _set_manual_aa1 = false;
                        _set_manual_aa2 = false;
                        _set_manual_aa3 = false;
                        _set_manual_aa4 = false;
                    }
                    break;
                case OID::RST_ENERGY:
                    set(response, varbind, match, index, &MIB::setRstEnergy);
                    dataCmd.command = CMD_RST_ENERGY;
                    dataCmd.pData = (const char*)&_rst_energy;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    _rst_energy = false;
                    break;
                case OID::SET_MANUAL_AA1:
                    set(response, varbind, match, index, &MIB::setManualAA1);
                    dataCmd.command = CMD_MANUAL_AA1;
                    dataCmd.pData = (const char*)&_set_manual_aa1;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::SET_MANUAL_AA2:
                    set(response, varbind, match, index, &MIB::setManualAA2);
                    dataCmd.command = CMD_MANUAL_AA2;
                    dataCmd.pData = (const char*)&_set_manual_aa2;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::SET_MANUAL_AA3:
                    set(response, varbind, match, index, &MIB::setManualAA3);
                    dataCmd.command = CMD_MANUAL_AA3;
                    dataCmd.pData = (const char*)&_set_manual_aa3;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    break;
                case OID::SET_MANUAL_AA4:
                    set(response, varbind, match, index, &MIB::setManualAA4);
                    dataCmd.command = CMD_MANUAL_AA4;
                    dataCmd.pData = (const char*)&_set_manual_aa4;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    
                    break;
                case OID::RST_FAIL_COUNT:
                    set(response, varbind, match, index, &MIB::setRstFailCount);
                    dataCmd.command = CMD_RST_FAIL_COUNT;
                    dataCmd.pData = (const char*)&_rst_fail_count;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    _rst_fail_count = false;
                    break;
                case OID::RST_DEVICE:
                    set(response, varbind, match, index, &MIB::serRstdevice);
                    dataCmd.command = CMD_RST_Device;
                    dataCmd.pData = (const char*)&_rst_device;
                    if(xQueueSend(command_queue, (void *)&dataCmd, 1000) == pdTRUE){
                        SERIAL_PRINTLN("[SNMP]->COMANDO PROCESADO");
                    }else{
                      SERIAL_PRINTLN("[SNMP]->COMANDO SE PERDIO");
                    }
                    _rst_device = false;
                    
                    break;
                case OID::COUNT:
                    // OID is unknown
                    // Set error, status and index
                    response->setError(SNMP::GEN_ERR, index + 1);
                    // Add OID to response with null value;
                    response->add(name);
                    break;
                default:
                    // The object can not be set
                    // Set error, status and index
                    response->setError(SNMP::NO_ACCESS, index + 1);
                    // Add OID to response with null value;
                    response->add(name);
                    break;
                }
            }
            return true;
        }
        //
        SERIAL_PRINTLN("NO COMUNITY");
        return false;
    }

    // Variables to hold value of MIB nodes
    const char* _descr = "DAECGE-Brightc Secuenciador";
    const char* _objectID = "";
    char* _contact = nullptr;
    char* _name = nullptr;
    char* _location = nullptr;
    //set/get
    char *_id = nullptr;
    char *_ip = nullptr;
    char *_t_max = nullptr;
    char *_t_min = nullptr;
    char *_t_alarma = nullptr;
    char *_t_bypass = nullptr;
    uint8_t _sec_dias = 0;
    uint8_t _cant_aa = 0;
    uint32_t _maxtime_cool_sp =0;
    uint32_t _sample_time = 0;
    uint8_t _bypass_logico = 0;
    uint8_t _manual = 0;
    uint8_t _rst_energy = 0;
    uint8_t _set_manual_aa1 = 0;
    uint8_t _set_manual_aa2 = 0;
    uint8_t _set_manual_aa3 = 0;
    uint8_t _set_manual_aa4 = 0;
    uint8_t _rst_fail_count = 0;
    uint8_t _rst_device = 0;
    //get
    char _tmp1[20] ;
    char _tmp2[20] ;
    char _hum[20] ;
    char _rms_v[20] ;
    char _pa[20] ;
    char _pr[20] ;
    char _pap[20] ;
    char _ea[20] ;
    char _er[20] ;
    char _fp[20] ;
    uint16_t _freq = 0;
    uint8_t _aaa1_as = 0;
    uint8_t _aaa1_aabp = 0;
    uint8_t _aaa2_as = 0;
    uint8_t _aaa2_aabp = 0;
    uint8_t _aaa3_as = 0;
    uint8_t _aaa3_aabp = 0;
    uint8_t _aaa4_as = 0;
    uint8_t _aaa4_aabp = 0;
    uint8_t _s_bypass_us = 0;
    uint8_t _a_tmp_high = 0;
    uint8_t _a_fire = 0;
    uint8_t _aa1_turn = 0;
    uint8_t _aa1_state = 0;
    uint8_t _aa1_help = 0;
    uint8_t _aa1_failcount = 0;
    uint8_t _aa2_turn = 0;
    uint8_t _aa2_state = 0;
    uint8_t _aa2_help = 0;
    uint8_t _aa2_failcount = 0;
    uint8_t _aa3_turn = 0;
    uint8_t _aa3_state = 0;
    uint8_t _aa3_help = 0;
    uint8_t _aa3_failcount = 0;
    uint8_t _aa4_turn = 0;
    uint8_t _aa4_state = 0;
    uint8_t _aa4_help = 0;
    uint8_t _aa4_failcount = 0;
    SNMP_Cmd_t dataCmd;
};
#endif
