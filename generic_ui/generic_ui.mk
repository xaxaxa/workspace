##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=generic_ui
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/xaxaxa/workspace"
ProjectPath            := "/dd/workspace/generic_ui"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=xaxaxa
Date                   :=12-10-26
CodeLitePath           :="/home/xaxaxa/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := --std=c++0x -g3 $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)/home/xaxaxa/workspace/include/" "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/generic_ui$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/generic_ui$(ObjectSuffix): generic_ui.C $(IntermediateDirectory)/generic_ui$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/dd/workspace/generic_ui/generic_ui.C" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/generic_ui$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/generic_ui$(DependSuffix): generic_ui.C
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/generic_ui$(ObjectSuffix) -MF$(IntermediateDirectory)/generic_ui$(DependSuffix) -MM "/dd/workspace/generic_ui/generic_ui.C"

$(IntermediateDirectory)/generic_ui$(PreprocessSuffix): generic_ui.C
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/generic_ui$(PreprocessSuffix) "/dd/workspace/generic_ui/generic_ui.C"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/generic_ui$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/generic_ui$(DependSuffix)
	$(RM) $(IntermediateDirectory)/generic_ui$(PreprocessSuffix)
	$(RM) $(OutputFile)


