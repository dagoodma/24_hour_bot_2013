#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
<<<<<<< HEAD
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/Bumper.o ${OBJECTDIR}/src/Drive.o ${OBJECTDIR}/src/IR.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/PORTS.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/RCServo.o ${OBJECTDIR}/src/roach.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/TapeSensor.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/Util.o ${OBJECTDIR}/_ext/1102761536/baller.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/src/AD.o.d ${OBJECTDIR}/src/Bumper.o.d ${OBJECTDIR}/src/Drive.o.d ${OBJECTDIR}/src/IR.o.d ${OBJECTDIR}/src/LED.o.d ${OBJECTDIR}/src/PORTS.o.d ${OBJECTDIR}/src/pwm.o.d ${OBJECTDIR}/src/RCServo.o.d ${OBJECTDIR}/src/roach.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/src/TapeSensor.o.d ${OBJECTDIR}/src/timers.o.d ${OBJECTDIR}/src/Util.o.d ${OBJECTDIR}/_ext/1102761536/baller.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/Bumper.o ${OBJECTDIR}/src/Drive.o ${OBJECTDIR}/src/IR.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/PORTS.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/RCServo.o ${OBJECTDIR}/src/roach.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/TapeSensor.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/Util.o ${OBJECTDIR}/_ext/1102761536/baller.o
=======
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/Bumper.o ${OBJECTDIR}/src/Drive.o ${OBJECTDIR}/src/IR.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/TapeSensor.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/Util.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/src/AD.o.d ${OBJECTDIR}/src/Bumper.o.d ${OBJECTDIR}/src/Drive.o.d ${OBJECTDIR}/src/IR.o.d ${OBJECTDIR}/src/LED.o.d ${OBJECTDIR}/src/pwm.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/src/TapeSensor.o.d ${OBJECTDIR}/src/timers.o.d ${OBJECTDIR}/src/Util.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/Bumper.o ${OBJECTDIR}/src/Drive.o ${OBJECTDIR}/src/IR.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/TapeSensor.o ${OBJECTDIR}/src/timers.o ${OBJECTDIR}/src/Util.o
>>>>>>> sm_code


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
${OBJECTDIR}/src/AD.o: src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/AD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/AD.o.d" -o ${OBJECTDIR}/src/AD.o src/AD.c   
	
${OBJECTDIR}/src/Bumper.o: src/Bumper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Bumper.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Bumper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Bumper.o.d" -o ${OBJECTDIR}/src/Bumper.o src/Bumper.c   
	
${OBJECTDIR}/src/Drive.o: src/Drive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Drive.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Drive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Drive.o.d" -o ${OBJECTDIR}/src/Drive.o src/Drive.c   
	
${OBJECTDIR}/src/IR.o: src/IR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/IR.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/IR.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/IR.o.d" -o ${OBJECTDIR}/src/IR.o src/IR.c   
	
${OBJECTDIR}/src/LED.o: src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/LED.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/LED.o.d" -o ${OBJECTDIR}/src/LED.o src/LED.c   
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c   
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/serial.o.d" -o ${OBJECTDIR}/src/serial.o src/serial.c   
	
${OBJECTDIR}/src/TapeSensor.o: src/TapeSensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/TapeSensor.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/TapeSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/TapeSensor.o.d" -o ${OBJECTDIR}/src/TapeSensor.o src/TapeSensor.c   
	
${OBJECTDIR}/src/timers.o: src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/timers.o.d" -o ${OBJECTDIR}/src/timers.o src/timers.c   
	
${OBJECTDIR}/src/Util.o: src/Util.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Util.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Util.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Util.o.d" -o ${OBJECTDIR}/src/Util.o src/Util.c   
	
${OBJECTDIR}/_ext/1102761536/baller.o: /Users/vharn/Documents/24_hour_bot_2013/24_hour_bot.X/src/baller.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1102761536 
	@${RM} ${OBJECTDIR}/_ext/1102761536/baller.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1102761536/baller.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/_ext/1102761536/baller.o.d" -o ${OBJECTDIR}/_ext/1102761536/baller.o /Users/vharn/Documents/24_hour_bot_2013/24_hour_bot.X/src/baller.c   
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
${OBJECTDIR}/src/AD.o: src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/AD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/AD.o.d" -o ${OBJECTDIR}/src/AD.o src/AD.c   
	
${OBJECTDIR}/src/Bumper.o: src/Bumper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Bumper.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Bumper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Bumper.o.d" -o ${OBJECTDIR}/src/Bumper.o src/Bumper.c   
	
${OBJECTDIR}/src/Drive.o: src/Drive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Drive.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Drive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Drive.o.d" -o ${OBJECTDIR}/src/Drive.o src/Drive.c   
	
${OBJECTDIR}/src/IR.o: src/IR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/IR.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/IR.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/IR.o.d" -o ${OBJECTDIR}/src/IR.o src/IR.c   
	
${OBJECTDIR}/src/LED.o: src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/LED.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/LED.o.d" -o ${OBJECTDIR}/src/LED.o src/LED.c   
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c   
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/serial.o.d" -o ${OBJECTDIR}/src/serial.o src/serial.c   
	
${OBJECTDIR}/src/TapeSensor.o: src/TapeSensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/TapeSensor.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/TapeSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/TapeSensor.o.d" -o ${OBJECTDIR}/src/TapeSensor.o src/TapeSensor.c   
	
${OBJECTDIR}/src/timers.o: src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/timers.o.d" -o ${OBJECTDIR}/src/timers.o src/timers.c   
	
${OBJECTDIR}/src/Util.o: src/Util.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/Util.o.d 
	@${FIXDEPS} "${OBJECTDIR}/src/Util.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/src/Util.o.d" -o ${OBJECTDIR}/src/Util.o src/Util.c   
	
${OBJECTDIR}/_ext/1102761536/baller.o: /Users/vharn/Documents/24_hour_bot_2013/24_hour_bot.X/src/baller.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1102761536 
	@${RM} ${OBJECTDIR}/_ext/1102761536/baller.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1102761536/baller.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"include/" -MMD -MF "${OBJECTDIR}/_ext/1102761536/baller.o.d" -o ${OBJECTDIR}/_ext/1102761536/baller.o /Users/vharn/Documents/24_hour_bot_2013/24_hour_bot.X/src/baller.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/24_hour_bot.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
