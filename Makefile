# *  Copyright(C) 2006 Neuros Technology International LLC. 
# *               <www.neurostechnology.com>
# *
# *  OSD master make file for various applications servers...MG 12-09-2006



ifndef PRJROOT
    $(error You must first source the BSP environment: "source neuros-env")
endif

OSD_IRPLUGIN     := osdir

OSD_APPS        := $(OSD_IRPLUGIN)

.PHONY: usage
usage:
	@echo "OSD Build Help"
	@echo "    make all        -- build the entire project "
	@echo "    make install    -- install to target "
	@echo "    make clean      -- clean project "
	@echo "    make cleanall   -- deep clean project "
	@echo


.PHONY: all cleanall clean install
all: 
	@for dir in  $(OSD_APPS); do \
		cd $$dir && ${NQMAKE} -nocache -recursive && \
		make all && cd .. || exit 1 ;\
	done;

clean:
	@for dir in  $(OSD_APPS); do \
		cd $$dir && ${NQMAKE} -nocache -recursive && \
		make clean && cd .. || exit 1 ;\
	done;

cleanall:
	@for dir in  $(OSD_APPS); do \
		cd $$dir && ${NQMAKE} -nocache -recursive && \
		make clean && cd .. || exit 1 ;\
		rm -rf $$dir/build; \
	done;

install:
	@for dir in  $(OSD_APPS); do \
		cd $$dir && ${NQMAKE} -nocache -recursive && \
		make install && cd .. || exit 1 ;\
	done;


