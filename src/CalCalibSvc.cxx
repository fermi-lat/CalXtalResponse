// Implementation file for CalCalibSvc
//
// $Header $
//
// Author: Zach Fewtrell

// LOCAL
#include "CalCalibSvc.h"

// GLAST

// EXTLIB
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

// STD

using namespace CalDefs;

static SvcFactory< CalCalibSvc > a_factory;
const ISvcFactory& CalCalibSvcFactory = a_factory; 

CalCalibSvc::CalCalibSvc(const string& name, ISvcLocator* Svc) 
  : Service(name,Svc)
{

  // declare the properties
  declareProperty("CalibDataSvc",    m_calibDataSvcName = "CalibDataSvc");
  declareProperty("DefaultFlavor",   m_defaultFlavor    = "ideal");
  declareProperty("FlavorIntNonlin", m_flavorIntNonlin  = "");
  declareProperty("FlavorAsym",      m_flavorAsym       = "");
  declareProperty("FlavorPed",       m_flavorPed        = "");
  declareProperty("FlavorMeVPerDac", m_flavorMPD        = "");
  declareProperty("FlavorTholdCI",   m_flavorTholdCI    = "");
  declareProperty("FlavorTholdMuon", m_flavorTholdMuon  = "");
}

StatusCode  CalCalibSvc::queryInterface (const IID& riid, void **ppvIF) {
  if (IID_ICalCalibSvc == riid) {
    *ppvIF = (ICalCalibSvc*)(this);
    return StatusCode::SUCCESS;
  } else return Service::queryInterface (riid, ppvIF);
}

const IID&  CalCalibSvc::type () const {return IID_ICalCalibSvc;}

StatusCode CalCalibSvc::initialize () 
{
  // Call super-class
  Service::initialize ();

  MsgStream msglog(msgSvc(), name());
  msglog << MSG::INFO << "initialize" << endreq;

  StatusCode sc;

  // Bind all of the properties for this service
  if ((sc = setProperties()).isFailure()) {
    msglog << MSG::ERROR << "Failed to set properties" << endreq;
    return sc;
  }

  // Post-init processing of jobOptions.
  // set default flavors unless otherwise specified
  if (!m_flavorIntNonlin.value().length())  m_flavorIntNonlin = m_defaultFlavor;
  if (!m_flavorAsym.value().length())       m_flavorAsym      = m_defaultFlavor;
  if (!m_flavorPed.value().length())        m_flavorPed       = m_defaultFlavor;
  if (!m_flavorMPD.value().length())        m_flavorMPD = m_defaultFlavor;
  if (!m_flavorTholdCI.value().length())    m_flavorTholdCI   = m_defaultFlavor;
  if (!m_flavorTholdMuon.value().length())  m_flavorTholdMuon  = m_defaultFlavor;

  msglog << MSG::DEBUG << "Initializing..."     << endreq;
  msglog << MSG::DEBUG << "  CalibDavaSvc   : " << m_calibDataSvcName  << endreq;
  msglog << MSG::DEBUG << "  DefaultFlavor  : " << m_defaultFlavor     << endreq;
  msglog << MSG::DEBUG << "  FlavorIntNonlin: " << m_flavorIntNonlin   << endreq;
  msglog << MSG::DEBUG << "  FlavorAsym     : " << m_flavorAsym        << endreq;
  msglog << MSG::DEBUG << "  FlavorPed      : " << m_flavorPed         << endreq;
  msglog << MSG::DEBUG << "  FlavorMeVPerDac: " << m_flavorMPD         << endreq;  
  msglog << MSG::DEBUG << "  FlavorTholdCI  : " << m_flavorTholdCI     << endreq;    
  msglog << MSG::DEBUG << "  FlavorTholdMuon: " << m_flavorTholdMuon   << endreq;   

  // Grab pointer to CalibDataSvc
  sc = service(m_calibDataSvcName, m_calibDataSvc, true);
  if ( !sc.isSuccess() ) {
    msglog << MSG::ERROR << "Could not get CalibDataSvc" << endreq;
    return sc;
  }

  // Initialize individual CalibItemMgr members.
  m_mpdMgr.initialize(*m_calibDataSvc,       m_flavorMPD,       msgSvc(), name());
  m_pedMgr.initialize(*m_calibDataSvc,       m_flavorPed,       msgSvc(), name());
  m_asymMgr.initialize(*m_calibDataSvc,      m_flavorAsym,      msgSvc(), name());
  m_intNonlinMgr.initialize(*m_calibDataSvc, m_flavorIntNonlin, msgSvc(), name());
  m_tholdMuonMgr.initialize(*m_calibDataSvc, m_flavorTholdMuon, msgSvc(), name());
  m_tholdCIMgr.initialize(*m_calibDataSvc,   m_flavorTholdCI,   msgSvc(), name());

  // Get ready to listen for BeginEvent
  IIncidentSvc* incSvc;
  sc = service("IncidentSvc", incSvc, true);
  if (sc.isSuccess() ) {
    int priority = 50;  // this should be lower priority (higher #?) than CalibDataSvc
    incSvc->addListener(this, "BeginEvent", priority);
  }
  else {
    msglog << MSG::ERROR << "Unable to find IncidentSvc" << endreq;
    return sc;
  }

  return StatusCode::SUCCESS;
}

/// Inform that a new incident has occured
void CalCalibSvc::handle ( const Incident& inc ) { 
  if ((inc.type() == "BeginEvent")) {
    m_mpdMgr.invalidate();
    m_pedMgr.invalidate();
    m_asymMgr.invalidate();
    m_intNonlinMgr.invalidate();
    m_tholdMuonMgr.invalidate();
    m_tholdCIMgr.invalidate();
  }
  return; 
}

StatusCode CalCalibSvc::finalize () {return StatusCode::SUCCESS;}

