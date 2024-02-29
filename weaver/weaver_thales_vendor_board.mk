#Kalama specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),kalama)
TARGET_USES_ST_ESE_WEAVER := false
ENABLE_ST_ESE_WEAVER_SPLIT_MANIFEST := false
endif

#Camano specific build rules
ifeq ($(TARGET_BOARD_PLATFORM),crow)
TARGET_USES_ST_ESE_WEAVER := false
ENABLE_ST_ESE_WEAVER_SPLIT_MANIFEST := false
endif
