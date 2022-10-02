# Build Keymaster open source vendor modules
ESE_ST_VENDOR_KEYMINT += android.hardware.security.keymint-service.strongbox-thales

ifeq ($(strip $(ENABLE_ESE_ST_KEYMINT_SPLIT_MANIFEST)),true)
ESE_ST_VENDOR_KEYMINT += android.hardware.security.keymint-service.strongbox-thales.xml
ESE_ST_VENDOR_KEYMINT += android.hardware.security.sharedsecret-service.strongbox-thales.xml
endif

ifeq ($(strip $(ENABLE_ESE_ST_KEYMINT_FEATURE_XML)),true)
PRODUCT_COPY_FILES += vendor/st/opensource/keymaster/keymint/2.0/hal/android.hardware.strongbox_keystore-thales.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.strongbox_keystore.xml
endif

ifeq ($(strip $(TARGET_USES_ESE_ST_KEYMINT)),true)
PRODUCT_PACKAGES += $(ESE_ST_VENDOR_KEYMINT)
PRODUCT_COPY_FILES += vendor/st/opensource/keymaster/keymint/2.0/hal/etc/hal_uuid_map_config.xml:$(TARGET_COPY_OUT_VENDOR)/etc/hal_uuid_map_config.xml
endif
