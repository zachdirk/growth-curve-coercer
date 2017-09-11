COMPILER=cl 
SRCDIR=src 
BUILDDIR=build 
TARGETDIR=bin 
INCLUDEDIR=include
DEBUGDIR=debug
CL_ARGS=/O2 /EHsc /c /I$(INCLUDEDIR) /Fo$(BUILDDIR)/
LK_ARGS=/Fe$(TARGETDIR)/MatchGrowthCurves.exe
!IF "$(DEBUG)" == "1"
CL_ARGS=/Od /EHsc /c /I$(INCLUDEDIR) /Fo$(BUILDDIR)/ /Zi /Fd$(DEBUGDIR)/MatchGrowthCurves.pdb
LK_ARGS=/Zi /DEBUG:FULL$(DEBUGDIR)/MatchGrowthCurves.pdb /Fe$(TARGETDIR)/MatchGrowthCurves.exe 
!ENDIF

OBJECTS=$(BUILDDIR)/main.obj $(BUILDDIR)/SiteIndex.obj  $(BUILDDIR)/SpeciesSubstitution.obj $(BUILDDIR)/Polygon.obj $(BUILDDIR)/GrowthCurve.obj $(BUILDDIR)/Logger.obj $(BUILDDIR)/Tools.obj $(BUILDDIR)/MakeOutput.obj

all: $(OBJECTS)
	$(COMPILER) $(LK_ARGS) $(OBJECTS)

clean:
	@echo Cleaning build, bin directories
	rm -f $(BUILDDIR)/*
	rm -f $(TARGETDIR)/*
	rm -f $(DEBUGDIR)/*

cleanbuild: clean all

$(BUILDDIR)/main.obj: $(SRCDIR)/main.cpp $(BUILDDIR)/SpeciesSubstitution.obj $(BUILDDIR)/SiteIndex.obj $(BUILDDIR)/Polygon.obj $(BUILDDIR)/GrowthCurve.obj $(BUILDDIR)/Logger.obj $(BUILDDIR)/Tools.obj $(BUILDDIR)/MakeOutput.obj
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/main.cpp

$(BUILDDIR)/SiteIndex.obj: $(SRCDIR)/SiteIndex.cpp $(BUILDDIR)/Polygon.obj $(BUILDDIR)/GrowthCurve.obj $(BUILDDIR)/Tools.obj
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/SiteIndex.cpp

$(BUILDDIR)/SpeciesSubstitution.obj: $(SRCDIR)/SpeciesSubstitution.cpp $(BUILDDIR)/Polygon.obj $(BUILDDIR)/Tools.obj
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/SpeciesSubstitution.cpp

$(BUILDDIR)/Polygon.obj: $(SRCDIR)/Polygon.cpp $(BUILDDIR)/GrowthCurve.obj $(BUILDDIR)/Tools.obj
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/Polygon.cpp

$(BUILDDIR)/GrowthCurve.obj: $(SRCDIR)/GrowthCurve.cpp $(BUILDDIR)/Tools.obj
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/GrowthCurve.cpp

$(BUILDDIR)/Tools.obj: $(SRCDIR)/Tools.cpp
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/Tools.cpp

$(BUILDDIR)/Logger.obj: $(SRCDIR)/Logger.cpp
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/Logger.cpp

$(BUILDDIR)/MakeOutput.obj: $(SRCDIR)/MakeOutput.cpp
	$(COMPILER) $(CL_ARGS) $(SRCDIR)/MakeOutput.cpp

