/*
*
*  $Id: icommand.cpp $
*  DeepWise Dicom Viewer
*
*  Copyright 2017-2030 DeepWise All rights reserved.
*  http://www.deepwise.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include <string>
#include <list>
#include <api/iexception.h>
//#include <api/controllers/iprogresscontroller.h>
#include "iproxynotificadorprogreso.h"
#include "icommand.h"

//---------------------------------------------------------------------------------------
//region Forward declarations
namespace GNC {
	class LanzadorComandos;
	class INotificadorProgreso;
	class CommandController;

	namespace GCS {
		class IComando;

	}
}
//endregion

namespace GNC {
	namespace GCS {

		//---------------------------------------------------------------------------------------
		IComandoParams::IComandoParams() {}
		IComandoParams::~IComandoParams() {}

		//---------------------------------------------------------------------------------------
		IComandoResult::IComandoResult() { }
		IComandoResult::~IComandoResult() { }

		//---------------------------------------------------------------------------------------
		IComando::IComando(IComandoParams* pParams, const std::string& name , void* pOwner, bool showProgreso) {
			m_pParams = pParams;
			m_pResult = NULL;
			m_pNotificadorProgreso = NULL;
            m_ShowProgreso = showProgreso;
			m_Abortado = false;
            m_Lasting = false;
			m_Nombre = name;
			m_pOwner = pOwner;
			if (m_Nombre.size() == 0)
			{
				m_Nombre = "Comando";
			}
			NotificarProgreso(0.0f, m_Nombre);
		}

		IComando::~IComando() {
			Free();
			OnDestroy();
		}

		void IComando::Free() {
			if (m_pResult != NULL)  {
				delete m_pResult;
				m_pResult = NULL;
			}
			if (m_pParams != NULL) {
				delete m_pParams;
				m_pParams = NULL;
			}
		}

		//endregion

		//---------------------------------------------------------------------------------------
		//region Interfaz especifica


		/** Notifica el progreso. Si devuelve true, el ejecutor dejara de procesarse **/
		bool IComando::NotificarProgreso(float progresoNormalizado, const std::string& texto)
		{
			if (m_Abortado) {
				return false;
			}

			//if (m_pNotificadorProgreso != NULL) {
			//	return m_pNotificadorProgreso->NotificarProgreso(progresoNormalizado, texto);
			//}

			return true;
		}

		/** Callback que puede ser sobrecargado por el sujeto para incluir acciones especificas cuando se aborta el comando **/
		void IComando::OnAbort() {}

		/** Callback que puede ser sobrecargado por el sujeto para incluir acciones especificas cuando se destruye el comando **/
		void IComando::OnDestroy() {}

		//endregion

		//---------------------------------------------------------------------------------------
		//region Getters y helpers
		int IComando::GetId() const {
			return m_Id;
		}

		void IComando::SetId(int id) {
			m_Id = id;
		}

		void IComando::SetOwner(void* pOwner)
		{
			m_pOwner = pOwner;
		}

		void* IComando::GetOwner() const
		{
			return m_pOwner;
		}

		const std::string& IComando::GetName() const
		{
			return m_Nombre;
		}

		IComandoParams* IComando::GetParams()
		{
			return m_pParams;
		}

		void IComando::Abort() {
			m_Abortado = true;
			OnAbort();
		}

		bool IComando::EstaAbortado() const {
			return m_Abortado;
		}

        void IComando::SetLasting(bool flag) {
            m_Lasting = flag;
        }

        bool IComando::GetLasting() const {
            return m_Lasting;
        }

		//endregion

		//---------------------------------------------------------------------------------------
		//region Notificacion
		GNC::INotificadorProgreso* IComando::GetNotificadorProgreso()
		{
			return m_pNotificadorProgreso;
		}

        void IComando::SetShowProgreso(bool showProgreso) {
            m_ShowProgreso = showProgreso;
        }

        bool IComando::GetShowProgreso() {
            return m_ShowProgreso;
        }

		// Este metodo debe ser invocado solo por el lanzador de comandos.
		void IComando::SetNotificadorProgreso(GNC::INotificadorProgreso* pNotificador)
		{
			m_pNotificadorProgreso = pNotificador;
		}
		//endregion

		//---------------------------------------------------------------------------------------
		//region "Gestion de Dependencias"


		void IComando::EsperaA(int idDep)
		{
			m_Dependencias.push_back(idDep);
		}

		void IComando::AbortaSi(int idConflicto)
		{
			m_Conflictos.push_back(idConflicto);
		}

		void IComando::CancelaA(int idReemplazo) {
			m_Reemplazos.push_back(idReemplazo);
		}

		///////////////IPersistentCommand////////////////
		IPersistentCommand::IPersistentCommand(const std::list<long> seriesId, const std::string description):
			IComando(NULL),
			m_seriesId(seriesId),
			m_idBBDD(-1),
			m_status(IPersistentCommand::Pending),
			m_description(description)
		{

		}

		IPersistentCommand::IPersistentCommand(long idBBDD, const std::list<long> seriesId, const std::string description):
			IComando(NULL),
			m_seriesId(seriesId),
			m_idBBDD(idBBDD),
			m_status(IPersistentCommand::Pending),
			m_description(description)
		{
		}

		IPersistentCommand::~IPersistentCommand()
		{
		}

		std::string IPersistentCommand::getClassUID()
		{
			throw new GNC::GCS::IException("Method has to be overridden");
		}

		long IPersistentCommand::getIdBBDD()
		{
			return m_idBBDD;
		}

		void IPersistentCommand::setIdBBDD(long id)
		{
			m_idBBDD = id;
		}

		const std::list<long>& IPersistentCommand::getSeriesId()
		{
			return m_seriesId;
		}

		const std::string& IPersistentCommand::getErrorDescription()
		{
			return m_errorDescription;
		}

		IPersistentCommand::TStatus IPersistentCommand::getStatus()
		{
			return m_status;
		}

		const std::string& IPersistentCommand::getDescription()
		{
			return m_description;
		}

		void IPersistentCommand::setStatus(IPersistentCommand::TStatus status)
		{
			m_status = status;
		}
	}
}
