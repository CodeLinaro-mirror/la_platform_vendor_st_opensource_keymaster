# Build Keymaster open source vendor modules
ifeq ($(strip $(TARGET_USES_ESE_ST_KEYMINT)),true)
PRODUCT_PACKAGES += android.hardware.security.keymint-service.strongbox-thales
endif

ifeq ($(strip $(TARGET_USES_ESE_ST_AUTHSECRET)),true)
PRODUCT_PACKAGES += android.hardware.authsecret-service.thales-qti
endif

ifeq ($(strip $(ENABLE_ESE_ST_KEYMINT_SPLIT_MANIFEST)),true)
  ifeq ($(strip $(TARGET_USES_ESE_ST_KEYMINT)),true)
  PRODUCT_PACKAGES += android.hardware.security.keymint-service.strongbox-thales.xml
  PRODUCT_PACKAGES += android.hardware.security.sharedsecret-service.strongbox-thales.xml
  endif

  ifeq ($(strip $(TARGET_USES_ESE_ST_AUTHSECRET)),true)
  PRODUCT_PACKAGES += android.hardware.authsecret-service.xml
  endif
endif

ifeq ($(strip $(ENABLE_ESE_ST_KEYMINT_FEATURE_XML)),true)
PRODUCT_COPY_FILES += vendor/st/opensource/keymaster/keymint/4.0/hal/android.hardware.strongbox_keystore-thales.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.strongbox_keystore.xml
endif
