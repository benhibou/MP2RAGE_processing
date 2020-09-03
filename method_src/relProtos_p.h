/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/initMeth.c */
void initMeth(void);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/loadMeth.c */
void loadMeth(const char *);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/parsRelations.c */
void deRelations(void);
void deRange(void);
void ExcPulse1EnumRelation(void);
void ExcPulse2EnumRelation(void);
void ExcPulse1AmplRel(void);
void ExcPulse2AmplRel(void);
void HandleRFPulseAmplitude(void);
void ExcPulse1Relation(void);
void ExcPulse1Range(void);
void ExcPulse2Relation(void);
void ExcPulse2Range(void);
void NAveragesRange(void);
void NAveragesRels(void);
void LocalEchoTimeRelation(void);
void SegmNumberRelations(void);
void SegmNumberRange(void);
void EffSWhRange(void);
void EffSWhRel(void);
void EchoPositionRels(void);
void EchoPositionRange(void);
void ReadSpoilerRel(void);
void SliceSpoilerRel(void);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/BaseLevelRelations.c */
void SetBaseLevelParam(void);
void SetBasicParameters(void);
void SetFrequencyParameters(void);
void SetGradientParameters(void);
void SetInfoParameters(void);
void SetMachineParameters(void);
void SetPpgParameters(void);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/RecoRelations.c */
void SetRecoParam(void);
void RecoDerive(void);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/backbone.c */
void backbone(void);
void UpdateSequenceTiming(void);
void UpdateEchoRepTime(void);
void UpdateGeometryMinima(double *, double *);
void UpdateReadSliceGradients(void);
void UpdatePhaseGradients(void);
void UpdateFrequencyOffsets(void);
void UpdateRFPulses(void);
void ControlGradientLimits(YesNo);
void UpdateEchoTime(void);
double minLoopRepetitionTime(void);
void UpdateRFSpoiling(void);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/MdeftPrepHandler.c */
void Mdeft_PrepSpoilGradRel(void);
void Mdeft_SliceSpoilerRel(void);
void Mdeft_ExcPulse1EnumRelation(void);
void Mdeft_ExcPulse1AmplRel(void);
void HandleMdeftExcPulseAmplitudes(void);
void Mdeft_ExcPulse1Range(void);
void Mdeft_ExcPulse1Relation(void);
void Mdeft_InvPulse1EnumRelation(void);
void Mdeft_InvPulse1AmplRel(void);
void HandleMdeftInvPulseAmplitudes(void);
void Mdeft_InvPulse1Range(void);
void Mdeft_InvPulse1Relation(void);
void InitMdeftPrepModule(void);
void UpdateMdeftPrepModule(double, YesNo, double);
/* /opt/PV6.0.1/prog/curdir/my/ParaVision/methods/src/myMP2RAGE/deriveVisu.c */
void deriveVisu(void);
