/*	File:		MyComponent.c	Contains:	simple component sample.	Written by: John Wang		Copyright:	Copyright � 1994-1999 by Apple Computer, Inc., All Rights Reserved.				You may incorporate this Apple sample source code into your program(s) without				restriction. This Apple sample source code has been provided "AS IS" and the				responsibility for its operation is yours. You are not permitted to redistribute				this Apple sample source code as "Apple sample source code" after having made				changes. If you're going to re-distribute the source, we require that you make				it clear in the source that the code was descended from Apple sample source				code, but that you've made changes.	Change History (most recent first):				7/28/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1				*/#ifdef THINK_C#define		applec#endif#include	<Resources.h>#include	<Memory.h>#include	<Errors.h>#include	<Components.h>#include	<LowMem.h>#include	"MyComponent.h"/* ------------------------------------------------------------------------- *///	Component entry point.pascal ComponentResult main(ComponentParameters *params, char **storage){	if ( kDEBUGME )		DebugStr("\pIn main()");		switch ( params->what ) {		case kComponentOpenSelect:			return ( CallComponentFunction(params, (ComponentFunctionUPP) MyOpen) );		case kComponentCloseSelect:			return ( CallComponentFunctionWithStorage(storage, params,					(ComponentFunctionUPP) MyClose) );					case kComponentCanDoSelect:			return ( CallComponentFunction(params, (ComponentFunctionUPP) MyCanDo) );		case kComponentVersionSelect: 			return ( CallComponentFunction(params, (ComponentFunctionUPP) MyVersion) );		case kComponentRegisterSelect: 			return ( CallComponentFunctionWithStorage(storage, params,					(ComponentFunctionUPP) MyRegister) );		default:			return ( paramErr );	}}/* ------------------------------------------------------------------------- *///	Required component calls.pascal ComponentResult MyOpen(ComponentInstance self){	PrivateGlobals	 	**myPrivateGlobals;		if ( kDEBUGME )		DebugStr("\pIn MyOpen()");			myPrivateGlobals = nil;		//	Create private variables.	myPrivateGlobals = (PrivateGlobals **) NewHandleClear(sizeof(PrivateGlobals));	if ( myPrivateGlobals == nil )		goto bail;		//	Initialize private variables.	(**myPrivateGlobals).self = self;		//	Since we've gotten here, everyt hings ok and we can set up the connection.	SetComponentInstanceStorage(self, (Handle) myPrivateGlobals);	return ( noErr );bail:	if ( myPrivateGlobals )		DisposeHandle((Handle) myPrivateGlobals);	return ( memFullErr );}pascal ComponentResult MyClose(Handle storage, ComponentInstance self){	PrivateGlobals	 	**myPrivateGlobals = (PrivateGlobals **) storage;	if ( kDEBUGME )		DebugStr("\pIn MyClose()");	//	Dispose of private variables.	if ( myPrivateGlobals )		DisposeHandle((Handle) myPrivateGlobals);		return ( noErr );}pascal ComponentResult MyCanDo(short selector){		if ( kDEBUGME )		DebugStr("\pIn MyCanDo()");		switch ( selector ) {			//	Required component calls.		case kComponentOpenSelect:		case kComponentCloseSelect:		case kComponentCanDoSelect:		case kComponentVersionSelect: 		case kComponentRegisterSelect: 			return ( true );		//	Not handled.		default:			return ( false );	}}pascal ComponentResult MyVersion(){	if ( kDEBUGME )		DebugStr("\pIn MyVersion()");	return ( (kMyComponentSpec<<16) | (kMyComponentVersion) );}pascal ComponentResult MyRegister(Handle storage){	PrivateGlobals	 			**myPrivateGlobals = (PrivateGlobals **) storage;	ComponentResourceHandle		theThing;	Component					theComponent;	short						saveResFile, compResFile;	OSErr						err;	short						i, numGnht;		if ( kDEBUGME )		DebugStr("\pIn MyRegister()");	saveResFile = CurResFile();		//	Make sure we have this resource file opened.	compResFile = OpenComponentResFile((Component) (**myPrivateGlobals).self);	UseResFile(compResFile);		//	Register all gnht resource.	numGnht = Count1Resources('gnht');	for ( i=1; i <= numGnht; i++ ) {		//	Get the component.		theThing = (ComponentResourceHandle) Get1IndResource('gnht', i);				//	Register the component.		if ( theThing ) {			theComponent = RegisterComponentResource(theThing, 1);			if ( theComponent == 0 )				DebugStr("\pDid not register properly!!!");		} else {			DebugStr("\pThis component is built wrong.  Can not find gnht with id 128!!!");		}				//	If we are installing new components, we would want them to be default.		err = SetDefaultComponent(theComponent, defaultComponentAnyFlagsAnyManufacturer);		if ( err != noErr )			DebugStr("\pCould not set default component!!!");	}		CloseComponentResFile(compResFile);	UseResFile(saveResFile);	return ( true );	//	Don't register me.}